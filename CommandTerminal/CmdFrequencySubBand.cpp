#include "CmdFrequencySubBand.h"

CmdFrequencySubBand::CmdFrequencySubBand(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Frequency Sub-band", "AT+FSB", "Set the frequency sub-band for US 915, (0:ALL, 1-8)"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(0-8)";
    _queryable = true;
}

uint32_t CmdFrequencySubBand::action(std::vector<std::string> args)
{

    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Frequency Sub Band: ");

        _serial.writef("%u\r\n", _dot->getFrequencySubBand());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        uint32_t band;
        sscanf(args[1].c_str(), "%lu", &band);

        if ((code = _dot->setFrequencySubBand(band)) != mDot::MDOT_OK) {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;
}

bool CmdFrequencySubBand::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        uint32_t band;
        if (sscanf(args[1].c_str(), "%u", &band) != 1) {
            setErrorMessage("Invalid arguments");
            return false;
        }

        if (band < mDot::FSB_ALL || band > mDot::FSB_8) {
            setErrorMessage("Invalid channel band, expects (0-8)");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}
