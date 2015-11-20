#include "CmdFrequencyBand.h"

CmdFrequencyBand::CmdFrequencyBand(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Frequency Band", "AT+FREQ", "Configured Frequency Band '868' or '915'"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(868,915)";
    _queryable = true;
}

uint32_t CmdFrequencyBand::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Frequency Band: ");

        _serial.writef("%s\r\n", mDot::FrequencyBandStr(_dot->getFrequencyBand()).c_str());
    }

#ifdef DEBUG_MAC
    else if (args.size() == 2)
    {
        int32_t code;
        uint8_t band = mDot::FB_915;

        if (mDot::FrequencyBandStr(mDot::FB_868).find(args[1]) != std::string::npos) {
            band = mDot::FB_868;
        }

        if ((code = _dot->setFrequencyBand(band)) != mDot::MDOT_OK) {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }
#endif

    return 0;
}

bool CmdFrequencyBand::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

#ifdef DEBUG_MAC
    if (args.size() == 2)
    {
        if (mDot::FrequencyBandStr(mDot::FB_868).find(args[1]) == std::string::npos &&
            mDot::FrequencyBandStr(mDot::FB_915).find(args[1]) == std::string::npos)
        {
            setErrorMessage("Invalid parameter, expects (868,915)");
            return false;
        }

        return true;
    }
#endif

    setErrorMessage("Invalid arguments");
    return false;
}
