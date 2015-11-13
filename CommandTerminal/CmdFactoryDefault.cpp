#include "CmdFactoryDefault.h"

CmdFactoryDefault::CmdFactoryDefault(mDot* dot) : Command(dot, "Reset Factory Defaults", "AT&F", "Reset current configuration to factory defaults")
{
    _help = std::string(text()) + ": " + std::string(desc());
}


uint32_t CmdFactoryDefault::action(std::vector<std::string> args)
{
    _dot->resetConfig();
    _dot->resetNetworkSession();
    return 0;
}

