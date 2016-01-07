#include "CmdNetworkJoinMode.h"

CmdNetworkJoinMode::CmdNetworkJoinMode(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Network Join Mode", "AT+NJM", "0: Manual configuration, 1: OTA Network Join (default: 1)"),
        _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(0-1)";
    _queryable = true;
}

uint32_t CmdNetworkJoinMode::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("%s: ", name());

        _serial.writef("%u\r\n", _dot->getJoinMode());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        uint8_t mode = (args[1] == "1") ? 1 : 0;
        if ((code = _dot->setJoinMode(mode)) != mDot::MDOT_OK)
        {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;
}

bool CmdNetworkJoinMode::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        if (!(args[1] == "0" || args[1] == "1"))
        {
            setErrorMessage("Invalid parameter, expects (0: Manual, 1: OTA)");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}
