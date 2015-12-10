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
