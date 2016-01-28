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

#ifndef __MODESWEEP_H__
#define __MODESWEEP_H__

#include "Mode.h"
#include "LayoutHelp.h"
#include "LayoutFile.h"
#include "LayoutConfirm.h"
#include "LayoutSweepProgress.h"
#include "LayoutSurveySuccess.h"
#include "LayoutSurveyFailure.h"
#include "LayoutSweepComplete.h"

typedef std::pair<uint8_t, uint32_t> point;

class ModeSweep : public Mode {
    public:
        ModeSweep(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors);
        ~ModeSweep();

        bool start();

    private:
        void displayHelp();
        void displaySuccess();
        std::vector<point> generatePoints();
        uint8_t payloadToRate(uint8_t payload);

        typedef enum {
            check_file = 0,
            confirm,
            show_help,
            in_progress,
            success,
            data,
            failure,
            complete
        } state;

        LayoutHelp _help;
        LayoutFile _file;
        LayoutConfirm _confirm;
        LayoutSweepProgress _progress;
        LayoutSurveySuccess _success;
        LayoutSurveyFailure _failure;
        LayoutSweepComplete _complete;
        DataItem _data;

        uint8_t _min_payload;
        uint8_t _max_payload;
        uint32_t _min_power;
        uint32_t _max_power;
        uint8_t _survey_current;
        uint8_t _survey_total;
        uint8_t _survey_success;
        uint8_t _survey_failure;
        std::vector<point> _points;
        Timer _display_timer;
};

#endif
