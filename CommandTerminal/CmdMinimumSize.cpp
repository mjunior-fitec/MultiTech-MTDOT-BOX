#include "CmdMinimumSize.h"

//SPECIAL NOTE: Minimum size is stored in the LoraConfig WakeInterval field. We decided to use 5 LoRaConfig locations,
// that are not used for the DotBox, for the 5 DotBox settings... +minsize, +maxsize, +minpwr, +maxpwr and +data.
CmdMinimumSize::CmdMinimumSize(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Minimum Size", "AT+MINSIZE", "Set the minimum payload size for sweep survey mode"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(11-242)";
    _queryable = true;
}

uint32_t CmdMinimumSize::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Minimum Size: ");
        _serial.writef("%lu\r\n", _dot->getWakeInterval());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        uint32_t size = 0;
        sscanf(args[1].c_str(), "%lu", &size);

        if ((code = _dot->setWakeInterval(size)) != mDot::MDOT_OK)
        {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;
}

bool CmdMinimumSize::verify(std::vector<std::string> args)
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
            setErrorMessage("Invalid minimum payload size, expects (11-242)");
            return false;
        }
        if (size > _dot->getWakeDelay())	//WakeDelay holds the MaxSize setting.
        {
            setErrorMessage("+MINSIZE cannot be greater than +MAXSIZE. Please increase +MAXSIZE first.");
            return false;
        }
        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}

