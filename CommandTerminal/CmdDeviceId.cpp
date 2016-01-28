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

#include "CmdDeviceId.h"
#include <algorithm>

CmdDeviceId::CmdDeviceId(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Device ID", "AT+DI", "Device EUI-64 (MSB) (unique, set at factory) (8 bytes)"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(hex:8)";
    _queryable = true;
}

uint32_t CmdDeviceId::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("%s: ", name());
        _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getDeviceId(), "-").c_str());
    }
#ifdef DEBUG_MAC    
    else if (args.size() == 2)
    {
        int32_t code;
        std::vector<uint8_t> NewEUI;

        // Read in the key components...
        readByteArray(args[1], NewEUI, EUI_LENGTH);

        if ((code = _dot->setDeviceId(NewEUI)) == mDot::MDOT_OK) {
            _serial.writef("Set %s: ", name());
            _serial.writef("%s\r\n", mts::Text::bin2hexString(NewEUI, "-").c_str());
        } else {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }
#endif
    return 0;
}

bool CmdDeviceId::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

#ifdef DEBUG_MAC
    if (args.size() == 2 && isHexString(args[1], 8))
        return true;

    setErrorMessage("Invalid id, expects (hex:8)");
#else
    setErrorMessage("Invalid arguments");
#endif

    return false;
}
