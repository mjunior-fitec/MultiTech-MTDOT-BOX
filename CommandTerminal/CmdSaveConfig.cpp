#include "CmdSaveConfig.h"

CmdSaveConfig::CmdSaveConfig(mDot* dot) : Command(dot, "Save Configuration", "AT&W", "Save configuration to flash memory")
{
    _help = std::string(text()) + ": " + std::string(desc());
}


uint32_t CmdSaveConfig::action(std::vector<std::string> args)
{
    if (!_dot->saveConfig()) {
      setErrorMessage("Failed to save to flash");
      return 1;
    }
    
    return 0;
}
