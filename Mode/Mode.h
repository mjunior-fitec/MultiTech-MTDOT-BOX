#ifndef __MODE_H__
#define __MODE_H__

#include "DOGS102.h"
#include "ButtonHandler.h"
#include "mDot.h"
#include "LoRaHandler.h"
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
            LoRaHandler::LoRaPing ping;
            uint8_t data_rate;
            uint32_t power;
        } DataItem;

        Mode(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora);
        ~Mode();

        virtual bool start() = 0;

    protected:
        bool deleteDataFile();
        bool appendDataFile(const DataItem& data);
        void updateData(DataItem& data, DataType type, bool status);
        uint32_t getIndex(DataType type);

        DOGS102* _lcd;
        ButtonHandler* _buttons;
        mDot* _dot;
        LoRaHandler* _lora;
        osThreadId _main_id;
        static const char* _file_name;
        uint32_t _index;
        uint8_t _band;
        uint8_t _sub_band;
        uint8_t _data_rate;
        uint8_t _power;
        uint32_t _next_tx;
        ButtonHandler::ButtonEvent _be;
        LoRaHandler::LoRaStatus _ls;
        LoRaHandler::LoRaPing _ping_result;
        uint8_t _state;
        bool _send_data;
};

#endif
