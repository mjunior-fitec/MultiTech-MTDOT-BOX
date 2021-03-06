/* Copyright (c) <2016> <MultiTech Systems>, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Mode.h"
#include "MTSLog.h"

/*
 * union for converting from 32-bit to 4 8-bit values
 */
union convert32 {
    int32_t f_s;		// convert from signed 32 bit int
    uint32_t f_u;		// convert from unsigned 32 bit int
    uint8_t t_u[4];		// convert to 8 bit unsigned array
}convertL;

/*
 * union for converting from 16- bit to 2 8-bit values
 */
union convert16 {
    int16_t f_s;		// convert from signed 16 bit int
    uint16_t f_u;		// convert from unsigned 16 bit int
    uint8_t t_u[2];		// convert to 8 bit unsigned array
} convertS;


Mode::Mode(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors)
  : _lcd(lcd),
    _buttons(buttons),
    _dot(dot),
    _lora(lora),
    _gps(gps),
    _sensors(sensors),
    _main_id(Thread::gettid()),
    _index(0),
    _band(_dot->getFrequencyBand()),
    _sub_band(_dot->getFrequencySubBand()),
    _data_rate(mDot::DR0),
    _power(2),
    _next_tx(0),
    _send_data(false),
    _gps_available(_gps->gpsDetected())
{}

Mode::~Mode() {}

bool Mode::deleteDataFile() {
    bool ret = true;

    // if survey data file exists, attempt to delete it
    std::vector<mDot::mdot_file> files = _dot->listUserFiles();
    for (std::vector<mDot::mdot_file>::iterator it = files.begin(); it != files.end(); it++)
        if (it->name == file_name) {
            if (! _dot->deleteUserFile(file_name))
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

    snprintf(id_buf, sizeof(id_buf), "%c%ld", (data.type == single) ? 'L' : 'S', data.index);

    // if we had GPS lock, format GPS data
    if (data.gps_lock) {
        snprintf(lat_buf, sizeof(lat_buf), "%d %d %d.%03d %c",
            abs(data.gps_latitude.degrees),
            data.gps_latitude.minutes,
            (data.gps_latitude.seconds * 6) / 1000,
            (data.gps_latitude.seconds * 6) % 1000,
            (data.gps_latitude.degrees > 0) ? 'N' : 'S');
        snprintf(lon_buf, sizeof(lon_buf), "%d %d %d.%03d %c",
            abs(data.gps_longitude.degrees),
            data.gps_longitude.minutes,
            (data.gps_longitude.seconds * 6) / 1000,
            (data.gps_longitude.seconds * 6) % 1000,
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
        float down_snr = (float)data.link.down.snr / 10.0;
        snprintf(stats_buf, sizeof(stats_buf), "%lu,%ld,%d,%2.1f",
            data.link.up.gateways,
            data.link.up.dBm,
            abs(data.link.down.rssi),
            down_snr);
    }

    size = snprintf(main_buf, sizeof(main_buf), "%s,%c,%ld,%s,%s,%s,%s,%s,%s,%lu\n",
        id_buf,
        data.status ? 'S' : 'F',
        data.gps_lock ? data.gps_sats : 0,
        (data.gps_lock) ? lat_buf : "",
        (data.gps_lock) ? lon_buf : "",
        (data.gps_lock) ? alt_buf : "",
        (data.gps_lock) ? time_buf : "",
        data.status ? stats_buf : ",,,",
        _dot->DataRateStr(data.data_rate).substr(2).c_str(),
        data.power);

    if (size < 0) {
        logError("failed to format survey data");
        return false;
    }

    if (! _dot->appendUserFile(file_name, (void*)main_buf, size)) {
        logError("failed to write survey data to file");
        return false;
    } else {
        logInfo("successfully wrote survey data to file\r\n\t%s", main_buf);
    }

    return true;
}

void Mode::updateData(DataItem& data, DataType type, bool status) {
    data.type = type;
    data.index = _index;
    data.status = status;
    data.gps_lock = _gps->getLockStatus();
    data.gps_sats = _gps->getNumSatellites();
    data.gps_longitude = _gps->getLongitude();
    data.gps_latitude = _gps->getLatitude();
    data.gps_altitude = _gps->getAltitude();
    data.gps_time = _gps->getTimestamp();
    data.link = _link_check_result;
    data.data_rate = _data_rate;
    data.power = _power;
}

void Mode::updateSensorData(SensorItem& data) {
    data.accel_data = _sensors->getAcceleration();
    data.baro_data = _sensors->getBarometer();
    data.lux_data_raw = _sensors->getLightRaw();
    data.pressure_raw = _sensors->getPressureRaw();
    data.light = _sensors->getLight();
    data.pressure = _sensors->getPressure();
    data.altitude = _sensors->getAltitude();
    data.temperature = _sensors->getTemp(SensorHandler::CELSIUS);
}

uint32_t Mode::getIndex(DataType type) {
    uint32_t index = 0;
    mDot::mdot_file file;
    size_t buf_size = 128;
    char buf[buf_size];
    bool done = false;
    char search;

    int read_offset;
    int read_size;
    int reduce = buf_size - 32;
    int bytes_read;
    int ret;
    int current;

    if (type == single)
        search = 'L';
    else
        search = 'S';

    file = _dot->openUserFile(file_name, mDot::FM_RDONLY);
    if (file.fd < 0) {
        logError("failed to open survey data file");
    } else {
        //logInfo("file size %d", file.size);
        if (file.size > buf_size) {
            read_offset = file.size - buf_size - 1;
            read_size = buf_size;
        } else {
            read_offset = 0;
            read_size = file.size;
        }

        while (! done) {
            if (read_offset == 0)
                done = true;

            //logInfo("reading from index %d, %d bytes", read_offset, read_size);

            if (! _dot->seekUserFile(file, read_offset, SEEK_SET)) {
                logError("failed to seek %d/%d", read_offset, file.size);
                return 0;
            }
            memset(buf, 0, buf_size);
            ret = _dot->readUserFile(file, (void*)buf, read_size);
            if (ret != read_size) {
                logError("failed to read");
                return 0;
            }
            //logInfo("read %d bytes [%s]", ret, buf);
	    bytes_read = file.size - read_offset;
	    //logInfo("read %d total bytes", bytes_read);

            // read_size - 1 is the last byte in the buffer
            for (current = read_size - 1; current >= 0; current--) {
		// generic case where a preceding newline exists
                if (buf[current] == '\n' && current != read_size - 1) {
                    int test = current;
                    //logInfo("found potential %d, %c", read_offset + current, buf[test + 1]);
                    if (buf[test + 1] == search) {
                        sscanf(&buf[test + 2], "%ld", &index);
                        done = true;
                        break;
                    }
		// special case where the index we're looking for is in the first entry - no newline
                } else if (current == 0 && bytes_read >= file.size) {
                    int test = current;
                    //logInfo("found potential %d, %c", read_offset + current, buf[test + 1]);
		    if (buf[test] == search) {
			sscanf(&buf[test + 1], "%ld", &index);
			done = true;
			break;
		    }
		}
            }

            read_offset = (read_offset - reduce > 0) ? read_offset - reduce : 0;
        }
        _dot->closeUserFile(file);
    }

    logInfo("returning index %d", index);

    return index;
}

std::vector<uint8_t> Mode::formatSurveyData(DataItem& data) {
    std::vector<uint8_t> send_data;
    uint8_t satfix;

    send_data.clear();
    send_data.push_back(0x1D);			// key for start of data structure
    send_data.push_back(0x1A);			// key for uplink QOS + RF Pwr
    convertS.f_s = data.link.up.gateways;
    send_data.push_back(convertS.t_u[1]);
    send_data.push_back(convertS.t_u[0]);
    send_data.push_back((data.link.up.dBm) & 0xFF);
    send_data.push_back(data.power);

    send_data.push_back(0x1B);			// key for downlink QOS
    convertS.f_s=data.link.down.rssi;
    send_data.push_back(convertS.t_u[1]);
    send_data.push_back(convertS.t_u[0]);
    send_data.push_back((data.link.down.snr/10) & 0xFF);

    // collect GPS data if GPS device detected
    if (_gps->gpsDetected() && ((_data_rate != mDot::SF_10) || (_band == mDot::FB_EU868))){
	    send_data.push_back(0x19);			// key for GPS Lock Status
	    satfix = (_gps->getNumSatellites() << 4 ) | (_gps->getFixStatus() & 0x0F );
	    send_data.push_back(satfix);

	    if (_gps->getLockStatus()){				    // if gps has a lock
		    // Send GPS data if GPS device locked
		    send_data.push_back(0x15);			    // key for GPS Latitude
		    send_data.push_back(data.gps_latitude.degrees);
		    send_data.push_back(data.gps_latitude.minutes);
		    convertS.f_s = data.gps_latitude.seconds;
		    send_data.push_back(convertS.t_u[1]);
		    send_data.push_back(convertS.t_u[0]);

		    send_data.push_back(0x16);			    // key for GPS Longitude
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
    send_data.clear();
    send_data.push_back(0x0E);			// key for Current Acceleration 3-Axis Value
    convertS.f_s = data.accel_data._x *4;	// shift data 2 bits while retaining sign
    send_data.push_back(convertS.t_u[1]);	// get 8 MSB of 14 bit value
    convertS.f_s = data.accel_data._y * 4;	// shift data 2 bits while retaining sign
    send_data.push_back(convertS.t_u[1]);	// get 8 MSB of 14 bit value
    convertS.f_s = data.accel_data._z * 4;	// shift data 2 bits while retaining sign
    send_data.push_back(convertS.t_u[1]);	// get 8 MSB of 14 bit value
    send_data.push_back(0x08);			// key for Current Pressure Value
    convertL.f_u = data.pressure_raw;		// pressure data is 20 bits unsigned
    send_data.push_back(convertL.t_u[2]);
    send_data.push_back(convertL.t_u[1]);
    send_data.push_back(convertL.t_u[0]);
    send_data.push_back(0x05);			// key for Current Ambient Light Value
    convertS.f_u = data.lux_data_raw;		// data is 16 bits unsigned
    send_data.push_back(convertS.t_u[1]);
    send_data.push_back(convertS.t_u[0]);
    send_data.push_back(0x0B);			// key for Current Temperature Value
    convertS.f_s = data.baro_data._temp; 	// temperature is signed 12 bit
    send_data.push_back(convertS.t_u[1]);
    send_data.push_back(convertS.t_u[0]);

    return send_data;
}

