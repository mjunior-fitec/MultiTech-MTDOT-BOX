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

#ifndef __MODE_H__
#define __MODE_H__

#include "DOGS102.h"
#include "ButtonHandler.h"
#include "mDot.h"
#include "LoRaHandler.h"
#include "GPSPARSER.h"
#include "SensorHandler.h"
#include "ISL29011.h"
#include "MMA845x.h"
#include "MPL3115A2.h"
#include "FileName.h"

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
	    bool gps_lock;
            int32_t gps_sats;
            GPSPARSER::longitude gps_longitude;
            GPSPARSER::latitude gps_latitude;
            int16_t gps_altitude;
            struct tm gps_time;
            LoRaHandler::LoRaLink link;
            uint8_t data_rate;
            uint32_t power;
        } DataItem;

        typedef struct {
            MMA845x_DATA accel_data;
            MPL3115A2_DATA baro_data;
            uint16_t lux_data_raw;
            uint32_t pressure_raw;
            float light;
            float pressure;
			float altitude;
			float temperature;			
        } SensorItem;		

        Mode(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors);
        ~Mode();

        virtual bool start() = 0;

    protected:
        bool deleteDataFile();
        bool appendDataFile(const DataItem& data);
        void updateData(DataItem& data, DataType type, bool status);
		void updateSensorData(SensorItem& data);
        uint32_t getIndex(DataType type);

        std::vector<uint8_t> formatSurveyData(DataItem& data);
        std::vector<uint8_t> formatSensorData(SensorItem& data);

        DOGS102* _lcd;
        ButtonHandler* _buttons;
        mDot* _dot;
        LoRaHandler* _lora;
	GPSPARSER* _gps;
	SensorHandler* _sensors;
        osThreadId _main_id;
        uint32_t _index;
        uint8_t _band;
        uint8_t _sub_band;
        uint8_t _data_rate;
        uint32_t _power;
        uint32_t _next_tx;
        ButtonHandler::ButtonEvent _be;
        LoRaHandler::LoRaStatus _ls;
        LoRaHandler::LoRaLink _link_check_result;
        uint8_t _state;
        bool _send_data;
	bool _gps_available;
	uint8_t _initial_data_rate;
	uint8_t _initial_power;
};

#endif
