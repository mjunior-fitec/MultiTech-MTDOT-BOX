#include "CmdNetworkKey.h"

CmdNetworkKey::CmdNetworkKey(mDot* dot, mts::MTSSerial& serial) :
    Command(dot, "Network Key", "AT+NK", "Configured network key/passphrase (App Key in LoraMac) ##  AT+NK=0,hex  AT+NK=1,passphrase (Net key = cmac(passphrase)) (16 bytes)"),
    _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(0,(hex:16)),(1,(string:128))";
    _queryable = true;
}

uint32_t CmdNetworkKey::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Network Key: ");

        _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getNetworkKey(), ".").c_str());
        if (!_dot->getNetworkPassphrase().empty())
            _serial.writef("Passphrase: '%s'\r\n", _dot->getNetworkPassphrase().c_str());

    }
    else if (args.size() == 3)
    {
        int32_t code;

        if (args[1].find("1") == 0 && args[1].size() == 1)
        {
            std::string text;
            if (args.size() > 3)
            {
                // passphrase was split on commas
                for (size_t i = 2; i < args.size(); i++)
                {
                    text.append(args[i]);
                    if (i < args.size() - 1)
                        text.append(",");
                }
            }
            else
            {
                text = args[2];
            }

            if ((code = _dot->setNetworkPassphrase(text)) == mDot::MDOT_OK)
            {
                _serial.writef("Set Network Passphrase: ");
                _serial.writef("%s\r\n", text.c_str());
            }
            else
            {
                std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
                setErrorMessage(error);
                return 1;
            }
        }
        else
        {
            std::vector<uint8_t> NewKey;
            readByteArray(args[2], NewKey, KEY_LENGTH);
            if ((code = _dot->setNetworkKey(NewKey)) == mDot::MDOT_OK)
            {
                _serial.writef("Set Network Key: ");
                _serial.writef("%s\r\n", mts::Text::bin2hexString(NewKey, ".").c_str());
            }
            else
            {
                std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
                setErrorMessage(error);
                return 1;
            }
        }
    }

    return 0;
}

bool CmdNetworkKey::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 3) {
        if (args[1] != "0" && args[1] != "1") {
            setErrorMessage("Invalid type, expects (0,1)");
            return false;
        }
        if (args[1] == "0" && !isHexString(args[2], 16)) {
            setErrorMessage("Invalid key, expects (hex:16)");
            return false;
        }

        if (args[1] == "1" && args[2].size() < 8) {
            setErrorMessage("Invalid name, expects minimum 8 characters");
            return false;
        }

        if (args[1] == "1" && (args[2].size() > 128 || args[2].size() < 8)) {
            setErrorMessage("Invalid passphrase, expects (string:8-128)");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}

