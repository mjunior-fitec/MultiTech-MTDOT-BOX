#include "CmdMaximumPower.h"

//SPECIAL NOTE: Maximum power is stored in the LoraConfig WakeMode field. We decided to use 5 LoRaConfig locations,
// that are not used for the DotBox, for the 5 DotBox settings... +minsize, +maxsize, +minpwr, +maxpwr and +data.
CmdMaximumPower::CmdMaximumPower(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Maximum Power", "AT+MAXPWR", "Set the maximum transmit power for survey sweep mode"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(2-20)";
    _queryable = true;
}

uint32_t CmdMaximumPower::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Maximum Power: ");
        _serial.writef("%lu\r\n", _dot->getWakeMode());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        uint32_t power = 0;
        sscanf(args[1].c_str(), "%lu", &power);
        if ((code = _dot->setWakeMode(power)) != mDot::MDOT_OK)
        {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;
}

bool CmdMaximumPower::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        uint32_t power = 0;
        if (sscanf(args[1].c_str(), "%lu", &power) != 1) {
            setErrorMessage("Invalid argument");
            return false;
        }

        if (power < 2 || power > 20)
        {
            setErrorMessage("Invalid maximum transmit power for sweep survey mode, expects (2-20)");
            return false;
        }
        if (power < _dot->getWakeTimeout())	//WakeTimeout holds the MinPower setting.
        {
            setErrorMessage("+MAXPWR cannot be less than +MINPWR. Please decrease +MINPWR first.");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}

