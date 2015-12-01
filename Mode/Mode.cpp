#include "Mode.h"
#include "MTSLog.h"

const char* Mode::_file_name = "SurveyData.txt";

/*
 * union for converting from 16- bit to 2 8-bit values
 */
union convert16 {
    int16_t f_s;		// convert from signed 16 bit int
    uint16_t f_u;		// convert from unsigned 16 bit int
    uint8_t t_u[2];		// convert to 8 bit unsigned array
} convertS;


Mode::Mode(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora)
  : _lcd(lcd),
    _buttons(buttons),
    _dot(dot),
    _lora(lora),
    _main_id(Thread::gettid()),
    _index(0),
    _band(_dot->getFrequencyBand()),
    _sub_band(_dot->getFrequencySubBand()),
    _data_rate(mDot::SF_7),
    _power(2),
    _next_tx(0),
    _send_data(false),
	_gpsUART(PA_2, PA_3),
	_mdot_gps(&_gpsUART)
{}

Mode::~Mode() {}

bool Mode::deleteDataFile() {
    bool ret = true;

    // if survey data file exists, attempt to delete it
    std::vector<mDot::mdot_file> files = _dot->listUserFiles();
    for (std::vector<mDot::mdot_file>::iterator it = files.begin(); it != files.end(); it++)
        if (it->name == _file_name) {
            if (! _dot->deleteUserFile(_file_name))
                ret = false;
            break;
        }

    return ret;
}

// ID, Status, Lock, Lat, Long, Alt, Time, RSSIup, SNRup, RSSIdown, SNRdown, DataRate, Power
bool Mode::appendDataFile(const DataItem& data) {
    char main_buf[256];
    char id_buf[16];
    char lat_buf[32];
    char lon_buf[32];
    char alt_buf[16];
    char time_buf[32];
    char stats_buf[32];
    size_t size;

    memset(main_buf, 0, sizeof(main_buf));
    memset(id_buf, 0, sizeof(id_buf));
    memset(lat_buf, 0, sizeof(lat_buf));
    memset(lon_buf, 0, sizeof(lon_buf));
    memset(alt_buf, 0, sizeof(alt_buf));
    memset(time_buf, 0, sizeof(time_buf));
    memset(stats_buf, 0, sizeof(stats_buf));

    snprintf(id_buf, sizeof(id_buf), "%c%ld", (data.type == single) ? 'P' : 'S', data.index);

    // if we had GPS lock, format GPS data
    if (data.lock > 0) {
        snprintf(lat_buf, sizeof(lat_buf), "%d %d %d.%03d %c",
            abs(data.gps_latitude.degrees),
            data.gps_latitude.minutes,
            (data.gps_latitude.seconds * 6) / 1000,
            (data.gps_latitude.seconds % 6) / 1000,
            (data.gps_latitude.degrees > 0) ? 'N' : 'S');
        snprintf(lon_buf, sizeof(lon_buf), "%d %d %d.%03d %c",
            abs(data.gps_longitude.degrees),
            data.gps_longitude.minutes,
            (data.gps_longitude.seconds * 6) / 1000,
            (data.gps_longitude.seconds % 6) / 1000,
            (data.gps_longitude.degrees > 0) ? 'E' : 'W');
        snprintf(alt_buf, sizeof(alt_buf), "%d",
            data.gps_altitude);
        snprintf(time_buf, sizeof(time_buf), "%02d:%02d:%02d %02d/%02d/%04d",
            data.gps_time.tm_hour,
            data.gps_time.tm_min,
            data.gps_time.tm_sec,
            data.gps_time.tm_mon + 1,
            data.gps_time.tm_mday,
            data.gps_time.tm_year + 1900);
    }

    if (data.status) {
        snprintf(stats_buf, sizeof(stats_buf), "%d,%d.%ld,%d,%d.%ld",
            data.ping.up.rssi,
            data.ping.up.snr / 10, abs(data.ping.up.snr) % 10,
            data.ping.down.rssi,
            data.ping.down.snr / 4, (abs(data.ping.down.snr) % 10) * 25);
    }

    size = snprintf(main_buf, sizeof(main_buf), "%s,%c,%ld,%s,%s,%s,%s,%s,%s,%lu\n",
        id_buf,
        data.status ? 'S' : 'F',
        data.lock,
        (data.lock > 0) ? lat_buf : "",
        (data.lock > 0) ? lon_buf : "",
        (data.lock > 0) ? alt_buf : "",
        (data.lock > 0) ? time_buf : "",
        data.status ? stats_buf : ",,,",
        _dot->DataRateStr(data.data_rate).substr(3).c_str(),
        data.power);

    if (size < 0) {
        logError("failed to format survey data");
        return false;
    }

    if (! _dot->appendUserFile(_file_name, (void*)main_buf, size)) {
        logError("failed to write survey data to file");
        return false;
    } else {
        logInfo("successfully wrote survey data to file");
    }

    return true;
}

void Mode::updateData(DataItem& data, DataType type, bool status) {
    data.type = type;
    data.index = _index;
    data.status = status;
    data.lock = 0;
	data.gps_longitude = _mdot_gps.getLongitude();
	data.gps_latitude = _mdot_gps.getLatitude();
	data.gps_altitude = _mdot_gps.getAltitude();
    data.ping = _ping_result;
    data.data_rate = _data_rate;
    data.power = _power;
}

std::vector<uint8_t> Mode::formatSurveyData(DataItem& data) {
    std::vector<uint8_t> send_data;
	uint8_t satfix;

	send_data.clear();
	send_data.push_back(0x1D);			// key for start of data structure
	send_data.push_back(0x1A);			// key for uplink QOS + RF Pwr
	convertS.f_s = data.ping.up.rssi;
	send_data.push_back(convertS.t_u[1]);
	send_data.push_back(convertS.t_u[0]);
	send_data.push_back((data.ping.up.snr/10) & 0xFF);
	send_data.push_back(data.power);

	send_data.push_back(0x1B);			// key for downlink QOS
	convertS.f_s=data.ping.down.rssi;
	send_data.push_back(convertS.t_u[1]);
	send_data.push_back(convertS.t_u[0]);
	send_data.push_back(data.ping.down.snr);

	// collect GPS data if GPS device detected
	if (_mdot_gps.gpsDetected() && ((_data_rate != mDot::SF_10) || (_band == mDot::FB_868))){
		send_data.push_back(0x19);			// key for GPS Lock Status
		satfix = (_mdot_gps.getNumSatellites() << 4 ) | (_mdot_gps.getFixStatus() & 0x0F );
		send_data.push_back(satfix);

		if (_mdot_gps.getLockStatus()){			// if gps has a lock
   			// Send GPS data if GPS device locked
			send_data.push_back(0x15);			// key for GPS Latitude
			send_data.push_back(data.gps_latitude.degrees);
			send_data.push_back(data.gps_latitude.minutes);
			convertS.f_s = data.gps_latitude.seconds;
			send_data.push_back(convertS.t_u[1]);
			send_data.push_back(convertS.t_u[0]);

			send_data.push_back(0x16);			// key for GPS Longitude
			convertS.f_s = data.gps_longitude.degrees;
			send_data.push_back(convertS.t_u[1]);
			send_data.push_back(convertS.t_u[0]);

			send_data.push_back(data.gps_longitude.minutes);
			convertS.f_s = data.gps_longitude.seconds;
			send_data.push_back(convertS.t_u[1]);
			send_data.push_back(convertS.t_u[0]);
		}
	}
	// key for end of data structure		
	send_data.push_back(0x1D);					

    return send_data;
}

std::vector<uint8_t> Mode::formatSensorData(SensorItem& data) {
    std::vector<uint8_t> send_data;

    return send_data;
}

