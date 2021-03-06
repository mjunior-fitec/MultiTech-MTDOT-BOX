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

#include "CmdDisplayConfig.h"
#include "dotbox_version.h"
#include "MTSLog.h"

std::string version = MTDOT_BOX_VERSION;

CmdDisplayConfig::CmdDisplayConfig(mDot* dot, mts::MTSSerial& serial)
:
  Command(dot, "Display Settings", "AT&V", "Displays current settings and status"),
  _serial(serial) {
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "TABLE";
}

uint32_t CmdDisplayConfig::action(std::vector<std::string> args) {
    _serial.writef("Firmware: \t\t%s\r\n", version.c_str());
    _serial.writef("Library : \t\t%s\r\n", _dot->getId().c_str());
	
    _serial.writef("Device ID:\t\t");
    _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getDeviceId(), ":").c_str());

    _serial.writef("Frequency Band:\t\t%s\r\n", mDot::FrequencyBandStr(_dot->getFrequencyBand()).c_str());
    if (_dot->getFrequencyBand() == mDot::FB_US915)
	_serial.writef("Frequency Sub Band:\t%u\r\n", _dot->getFrequencySubBand());

    _serial.writef("Public Network:\t\t%s\r\n", _dot->getPublicNetwork() ? "on" : "off");

    _serial.writef("Network Address:\t%s\r\n", mts::Text::bin2hexString(_dot->getNetworkAddress()).c_str());

    _serial.writef("Network ID:\t\t");
    _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getNetworkId(), ":").c_str());

    _serial.writef("Network ID Passphrase:\t%s\r\n", _dot->getNetworkName().c_str());

    _serial.writef("Network Key:\t\t");
    _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getNetworkKey(), ".").c_str());

    _serial.writef("Network Key Passphrase:\t%s\r\n", _dot->getNetworkPassphrase().c_str());

    _serial.writef("Network Session Key:\t");
    _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getNetworkSessionKey(), ".").c_str());

    _serial.writef("Data Session Key:\t");
    _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getDataSessionKey(), ".").c_str());

    _serial.writef("Network Join Mode:\t%s\r\n", mDot::JoinModeStr(_dot->getJoinMode()).c_str());

    _serial.writef("Tx Data Rate:\t\t%s\r\n", mDot::DataRateStr(_dot->getTxDataRate()).c_str());
    _serial.writef("Tx Power:\t\t%u\r\n", _dot->getTxPower());
    //_serial.writef("Log Level:\t\t%ld\r\n", _dot->getLogLevel());
    _serial.writef("Log Level:\t\t%ld\r\n", mts::MTSLog::TRACE_LEVEL);

    _serial.writef("Maximum Size:\t\t%u\r\n",	_dot->getWakeDelay());		//DotBox +MaxSize is stored here.
    _serial.writef("Minimum Size:\t\t%u\r\n",	_dot->getWakeInterval());	//DotBox +MinSize is stored here.
    _serial.writef("Maximum Power:\t\t%u\r\n",	_dot->getWakeMode());		//DotBox +MaxPwr is stored here.
    _serial.writef("Minimum Power:\t\t%u\r\n",	_dot->getWakeTimeout());	//DotBox +MinPwr is stored here.
    _serial.writef("Data:\t\t\t%d\r\n",	_dot->getStartUpMode());			//DotBox +Data is stored here.


    return 0;
}

