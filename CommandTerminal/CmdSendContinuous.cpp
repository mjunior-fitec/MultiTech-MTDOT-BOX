#include "CmdSendContinuous.h"

CmdSendContinuous::CmdSendContinuous(mDot* dot)
: Command(dot, "Send Continuous", "AT+SENDC", "Send un-modulated data continuously") {
}

uint32_t CmdSendContinuous::action(std::vector<std::string> args) {

    _dot->sendContinuous(true);

    return 0;
}

