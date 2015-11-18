#ifndef __MODE_H__
#define __MODE_H__

#include "DOGS102.h"
#include "ButtonHandler.h"
#include "GPSPARSER.h"

class Mode {
    public:
        typedef enum {
            single = 0,
            sweep
        } DataType;

        typedef struct {
            DataType type;
            int32_t index;
            bool status;
            int32_t lock;
            GPSPARSER::longitude gps_longitude;
            GPSPARSER::latitude gps_latitude;
            int16_t gps_altitude;
            struct tm gps_time;
            int16_t rssi_up;
            int16_t snr_up;
            int16_t rssi_down;
            int16_t snr_down;
            uint8_t data_rate;
            uint8_t power;
        } DataItem;

        Mode(DOGS102* lcd, ButtonHandler* buttons);
        ~Mode();

        virtual bool start() = 0;

    protected:
        bool deleteDataFile();
        bool appendDataFile(const DataItem& data);

        DOGS102* _lcd;
        ButtonHandler* _buttons;
        uint32_t _index;
        osThreadId _main_id;
};

#endif
