#include "CmdMaximumSize.h"

CmdMaximumSize::CmdMaximumSize(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Maximum Size", "AT+MAXSIZE", "Set the maximum payload size for sweep survey mode"), _serial(serial)
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
//ToDo: Change from _dot->getTxPower() to the structure we will use for this.
        _serial.writef("%lu\r\n", _dot->getTxPower());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        uint32_t power = 0;
        sscanf(args[1].c_str(), "%lu", &power);

        if ((code = _dot->setTxPower(power)) != mDot::MDOT_OK)
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
        uint32_t power = 0;
        if (sscanf(args[1].c_str(), "%lu", &power) != 1) {
            setErrorMessage("Invalid argument");
            return false;
        }

        if (power < 11 || power > 242)
        {
            setErrorMessage("Invalid maximum payload size, expects (11-242)");
            return false;
        }
//ToDo: Output warning if < minimum size.
        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}

