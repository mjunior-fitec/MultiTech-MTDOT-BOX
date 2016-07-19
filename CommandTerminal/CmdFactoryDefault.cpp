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

#include "CmdFactoryDefault.h"

CmdFactoryDefault::CmdFactoryDefault(mDot* dot) : Command(dot, "Reset Factory Defaults", "AT&F", "Reset current configuration to factory defaults")
{
    _help = std::string(text()) + ": " + std::string(desc());
}


uint32_t CmdFactoryDefault::action(std::vector<std::string> args)
{
    _dot->resetConfig();

//Factory defaults for the DotBox.
    _dot->setTxDataRate(mDot::DR0);
    _dot->setFrequencySubBand(1);
    _dot->setWakeDelay(242);		//DotBox +MaxSize is stored here. Default is 242.
    _dot->setWakeInterval(11);		//DotBox +MinSize is stored here. Default is 11.
    _dot->setWakeMode(20);		//DotBox +MaxPwr is stored here. Default is 20.
    _dot->setWakeTimeout(2);		//DotBox +MinPwr is stored here. Default is 2.
    _dot->setStartUpMode(0);		//DotBox +Data is stored here. Default is 0.
    _dot->setVerbose(1);
    _dot->setEcho(1);
    _dot->setAck(1);
    _dot->setJoinMode(mDot::OTA);
    _dot->setNetworkName("MultiTech");
    _dot->setNetworkPassphrase("MultiTech");
	
    _dot->resetNetworkSession();
    return 0;
}

