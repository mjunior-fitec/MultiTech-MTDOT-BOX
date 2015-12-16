#include "CmdDummy.h"

CmdDummy::CmdDummy(mDot* dot, const char* name, const char* txt, const char* dsc) : Command(dot, name, txt, dsc)
{
    _help = std::string(text()) + ": " + std::string(desc());
}


uint32_t CmdDummy::action(std::vector<std::string> args)
{
    return 0;
}

