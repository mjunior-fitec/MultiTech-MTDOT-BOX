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

#include "CmdNetworkAddress.h"
#include <algorithm>

CmdNetworkAddress::CmdNetworkAddress(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Network Address", "AT+NA", "Network address (devAddr in LoraMac) (4 bytes)"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(hex:4)";
    _queryable = true;
}

uint32_t CmdNetworkAddress::action(std::vector<std::string> args)
{
    std::vector<uint8_t> addr;

    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Network Address: ");

        addr = _dot->getNetworkAddress();

        _serial.writef("%02x:%02x:%02x:%02x\r\n", addr[0], addr[1], addr[2], addr[3]);
    }
    else if (args.size() == 2)
    {
        int32_t code;
        uint8_t temp;
        uint32_t step = 2;

        if (args[1].find(":") != std::string::npos || args[1].find(".") != std::string::npos || args[1].find("-") != std::string::npos)
            step = 3;

        // Convert the ASCII hex data to binary...
        for (size_t i = 0; i < args[1].size(); i += step) 
        {
            sscanf(&args[1][i], "%02x", &temp);
            addr.push_back(temp);
        }

        if ((code = _dot->setNetworkAddress(addr)) == mDot::MDOT_OK) {
            _serial.writef("Set Network Address: ");
            _serial.writef("%02x:%02x:%02x:%02x\r\n", addr[0], addr[1], addr[2], addr[3]);
        } else {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;
}

bool CmdNetworkAddress::verify(std::vector<std::string> args)
{
    if (args.size() == 1) {
        return true;
    }

    if (args.size() == 2) {
        if (!isHexString(args[1], 4))
        {
            setErrorMessage("Invalid address, expects (hex:4)");
            return false;
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}
