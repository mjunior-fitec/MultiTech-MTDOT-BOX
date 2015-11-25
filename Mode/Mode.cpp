#include "Mode.h"
#include "MTSLog.h"

const char* Mode::_file_name = "SurveyData.txt";

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
    _send_data(false)
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
        snprintf(lat_buf, sizeof(lat_buf), "%3d %2d %2d.%03d %c",
            abs(data.gps_latitude.degrees),
            data.gps_latitude.minutes,
            (data.gps_latitude.seconds * 6) / 1000,
            (data.gps_latitude.seconds % 6) / 1000,
            (data.gps_latitude.degrees > 0) ? 'N' : 'S');
        snprintf(lon_buf, sizeof(lon_buf), "%3d %2d %2d.%03d %c",
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
        float up_snr = (float)data.ping.up.snr / 10.0;
        float down_snr = (float)data.ping.down.snr / 4.0;
        snprintf(stats_buf, sizeof(stats_buf), "%3d,%2.1f,%3d,%2.1f",
            abs(data.ping.up.rssi),
            up_snr,
            abs(data.ping.down.rssi),
            down_snr);
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
        logInfo("successfully wrote survey data to file\r\n\t%s", main_buf);
    }

    return true;
}

void Mode::updateData(DataItem& data, DataType type, bool status) {
    data.type = type;
    data.index = _index;
    data.status = status;
    data.lock = 0;
    // TODO add GPS info
    data.ping = _ping_result;
    data.data_rate = _data_rate;
    data.power = _power;
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
        search = 'P';
    else
        search = 'S';

    file = _dot->openUserFile(_file_name, mDot::FM_RDONLY);
    if (file.fd < 0) {
        logError("failed to open survey data file");
    } else {
        logInfo("file size %d", file.size);
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

            logInfo("reading from index %d, %d bytes", read_offset, read_size);

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
            logInfo("read %d bytes [%s]", ret, buf);

            // read_size - 1 is the last byte in the buffer
            for (current = read_size - 1; current >= 0; current--) {
                if ((buf[current] == '\n' && current != read_size - 1) || current == 0) {
                    int test = current;
                    logInfo("found potential %d, %c", read_offset + current, buf[test + 1]);
                    if (buf[test + 1] == search) {
                        logInfo("reading index");
                        sscanf(&buf[test + 2], "%ld", &index);
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

