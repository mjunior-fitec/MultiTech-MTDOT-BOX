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

#include "CmdIdentification.h"
#include "dotbox_version.h"

CmdIdentification::CmdIdentification(mDot* dot, mts::MTSSerial& serial) : Command(dot, "Request Id", "ATI", "Request Identification"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
}

uint32_t CmdIdentification::action(std::vector<std::string> args)
{
    std::string version = MTDOT_BOX_VERSION;
#ifdef DEBUG_MAC
    version += "-debug";
#endif
    _serial.writef("MultiTech mDot\r\n");
    _serial.writef("Firmware: %s\r\n", version.c_str());
    _serial.writef("Library : %s\r\n", _dot->getId().c_str());

    return 0;
}
