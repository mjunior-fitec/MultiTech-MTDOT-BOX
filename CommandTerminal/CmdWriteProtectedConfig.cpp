#include "CmdWriteProtectedConfig.h"

CmdWriteProtectedConfig::CmdWriteProtectedConfig(mDot* dot) :
        Command(dot, "Write Protected Config", "AT&WP", "Write protected config to flash (DeviceId and Frequency Band)")
{
    _help = std::string(text()) + ": " + std::string(desc());
}

uint32_t CmdWriteProtectedConfig::action(std::vector<std::string> args)
{
    if (!_dot->saveProtectedConfig()) {
      setErrorMessage("Failed to save to flash");
      return 1;
    }

    return 0;
}
