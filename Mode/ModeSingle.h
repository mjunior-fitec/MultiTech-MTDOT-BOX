#ifndef __MODESINGLE_H__
#define __MODESINGLE_H__

#include "Mode.h"
#include "LayoutHelp.h"
#include "LayoutFile.h"
#include "LayoutConfirm.h"
#include "LayoutSurveyProgress.h"
#include "LayoutSurveySuccess.h"
#include "LayoutSurveyFailure.h"

class ModeSingle : public Mode {
    public:
        ModeSingle(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps);
        ~ModeSingle();

        bool start();

    private:
        void displayHelp();
        void displaySuccess();
        std::string formatRatePower();
        void incrementRatePower();

        typedef enum {
            check_file = 0,
            confirm,
            show_help,
            in_progress,
            success,
            data,
            failure
        } state;

        LayoutHelp _help;
        LayoutFile _file;
        LayoutConfirm _confirm;
        LayoutSurveyProgress _progress;
        LayoutSurveySuccess _success;
        LayoutSurveyFailure _failure;
        DataItem _data;
};

#endif
