#ifndef __MODESINGLE_H__
#define __MODESINGLE_H__

#include "Mode.h"
#include "mDot.h"
#include "LoRaHandler.h"
#include "LayoutHelp.h"
#include "LayoutFile.h"
#include "LayoutConfirm.h"
#include "LayoutSurveyProgress.h"
#include "LayoutSurveySuccess.h"
#include "LayoutSurveyFailure.h"

class ModeSingle : public Mode {
    public:
        ModeSingle(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora);
        ~ModeSingle();

        bool start();

    private:
        void displayHelp();
        std::string formatNewRatePower();
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
        mDot* _dot;
        LoRaHandler* _lora;
        uint8_t _band;
        uint8_t _sub_band;
        uint8_t _data_rate;
        uint8_t _power;
        uint32_t _next_tx;
        ButtonHandler::ButtonEvent _be;
        LoRaHandler::LoRaStatus _ls;
        uint8_t _state;
        bool _send_data;
};

#endif
