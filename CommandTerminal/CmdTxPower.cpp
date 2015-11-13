#include "CmdTxPower.h"

CmdTxPower::CmdTxPower(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Tx Power", "AT+TXP", "Set the Tx power for all channels"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(2-20)";
    _queryable = true;
}

uint32_t CmdTxPower::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Tx Power: ");

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

bool CmdTxPower::verify(std::vector<std::string> args)
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
            setErrorMessage("Invalid power, expects (2-20)");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}

