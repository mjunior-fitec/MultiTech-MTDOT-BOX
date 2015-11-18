#include "CmdIdentification.h"
#include "version.h"

CmdIdentification::CmdIdentification(mDot* dot, mts::MTSSerial& serial) : Command(dot, "Request Id", "ATI", "Request Identification"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
}

uint32_t CmdIdentification::action(std::vector<std::string> args)
{
    std::string version = MTDOT_BOX_VERSION;
#ifdef DEBUG_MAC
    version += "-debug";
#endif
    _serial.writef("MultiTech mDot\r\n");
    _serial.writef("Firmware: %s\r\n", version.c_str());
    _serial.writef("Library : %s\r\n", _dot->getId().c_str());

    return 0;
}
