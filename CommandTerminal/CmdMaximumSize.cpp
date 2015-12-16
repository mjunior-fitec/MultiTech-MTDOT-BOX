#include "CmdMaximumSize.h"

//SPECIAL NOTE: Maximum size is stored in the LoraConfig WakeDelay field. We decided to use 5 LoRaConfig locations,
// that are not used for the DotBox, for the 5 DotBox settings... +minsize, +maxsize, +minpwr, +maxpwr and +data.
CmdMaximumSize::CmdMaximumSize(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Maximum Size", "AT+MAXSIZE", "Set the maximum payload size for survey sweep mode"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(11-242)";
    _queryable = true;
}

uint32_t CmdMaximumSize::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Maximum Size: ");
        _serial.writef("%lu\r\n", _dot->getWakeDelay());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        uint32_t size = 0;
        sscanf(args[1].c_str(), "%lu", &size);

        if ((code = _dot->setWakeDelay(size)) != mDot::MDOT_OK)
        {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;
}

bool CmdMaximumSize::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        uint32_t size = 0;
        if (sscanf(args[1].c_str(), "%lu", &size) != 1) {
            setErrorMessage("Invalid argument");
            return false;
        }

        if (size < 11 || size > 242)
        {
            setErrorMessage("Invalid maximum payload size, expects (11-242)");
            return false;
        }
        if (size < _dot->getWakeInterval())	//WakeInterval holds the MinSize setting.
        {
            setErrorMessage("+MAXSIZE cannot be less than +MINSIZE. Please decrease +MINSIZE first.");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}

