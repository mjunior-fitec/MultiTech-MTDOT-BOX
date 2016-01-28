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

#include "CmdDataSessionKey.h"

CmdDataSessionKey::CmdDataSessionKey(mDot* dot, mts::MTSSerial& serial) :
    Command(dot, "Data Session Key", "AT+DSK", "Data session encryption key (16 bytes)"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(hex:16)";
    _queryable = true;
}

uint32_t CmdDataSessionKey::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Data Session Key: ");
        _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getDataSessionKey(), ".").c_str());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        std::vector<uint8_t> NewKey;

        // Read in the key components...
        readByteArray(args[1], NewKey, KEY_LENGTH);

        if ((code = _dot->setDataSessionKey(NewKey)) == mDot::MDOT_OK) {
            _serial.writef("Set Data Session Key: ");
            _serial.writef("%s\r\n", mts::Text::bin2hexString(NewKey, ".").c_str());
        } else {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;
}

bool CmdDataSessionKey::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        if (!isHexString(args[1], 16)) {
            setErrorMessage("Invalid key, expects (hex:16)");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}
