#include "CmdData.h"

//SPECIAL NOTE: +Data is stored in the LoraConfig StartUpMode field. We decided to use 5 LoRaConfig locations,
// that are not used for the DotBox, for the 5 DotBox settings... +minsize, +maxsize, +minpwr, +maxpwr and +data.
CmdData::CmdData(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Data", "AT+DATA", "Enable/disable sending survey data results packet to the network server upon each successful survey. (0: off, 1: on)"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(0,1)";
    _queryable = true;
}

uint32_t CmdData::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("%s: ", name());
        _serial.writef("%d\r\n", _dot->getStartUpMode());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        bool enable = (args[1] == "1");

        if ((code = _dot->setStartUpMode(enable)) != mDot::MDOT_OK) {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;

}

bool CmdData::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        if (args[1] != "1" && args[1] != "0") {
            setErrorMessage("Invalid parameter, expects (0: off, 1: on)");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;

}

