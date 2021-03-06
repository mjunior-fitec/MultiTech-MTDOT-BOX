/* Copyright (c) <2016> <MultiTech Systems>, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "CmdNetworkId.h"

CmdNetworkId::CmdNetworkId(mDot* dot, mts::MTSSerial& serial) :
    Command(dot, "Network ID", "AT+NI", "Configured Network EUI/Name (App EUI in LoraMac) AT+NI=0,hex AT+NI=1,network_name  (Net ID = crc64(network_name)) (8 bytes)"),
            _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(0,(hex:8)),(1,(string:128))";
    _queryable = true;
}

uint32_t CmdNetworkId::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Network ID: ");

        _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getNetworkId(), ":").c_str());

        if (!_dot->getNetworkName().empty())
            _serial.writef("Passphrase: '%s'\r\n", _dot->getNetworkName().c_str());
    }
    else
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

            if ((code = _dot->setNetworkName(text)) == mDot::MDOT_OK)
            {
                _serial.writef("Set Network Name: ");
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
            readByteArray(args[2], NewKey, EUI_LENGTH);
            if ((code = _dot->setNetworkId(NewKey)) == mDot::MDOT_OK)
            {
                _serial.writef("Set Network ID: ");
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

bool CmdNetworkId::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 3) {
        if (args[1] != "0" && args[1] != "1") {
            setErrorMessage("Invalid type, expects (0,1)");
            return false;
        }
        if (args[1] == "0" && !isHexString(args[2], 8)) {
            setErrorMessage("Invalid ID, expects (hex:8");
            return false;
        }

        if (args[1] == "1" && args[2].size() < 8) {
            setErrorMessage("Invalid name, expects minimum 8 characters");
            return false;
        }

        if (args[1] == "1" && args[2].size() > 128) {
            setErrorMessage("Invalid name, expects (string:128)");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}
