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

#include "CmdTxDataRate.h"

CmdTxDataRate::CmdTxDataRate(mDot* dot, mts::MTSSerial& serial) :
    Command(dot, "Tx Data Rate", "AT+TXDR", "Set the Tx data rate for LoRa demo mode"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());

    if (_dot->getFrequencyBand() == mDot::FB_915)
        _usage = "(7-10)";
    else
        _usage = "(7-12)";
    _queryable = true;
}

uint32_t CmdTxDataRate::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Tx Data Rate: ");

        _serial.writef("%s\r\n", mDot::DataRateStr(_dot->getTxDataRate()).c_str());
    }
    else if (args.size() == 2)
    {
        int32_t code;
        uint8_t datarate = 0;
        uint8_t i;

        for (i = 0; i < 8; i++)
        {
            if (mDot::DataRateStr(i).find(args[1].c_str()) != std::string::npos)
            {
                datarate = i;
                break;
            }
        }

        if ((code = _dot->setTxDataRate(datarate)) != mDot::MDOT_OK)
        {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }

    return 0;
}

bool CmdTxDataRate::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

    if (args.size() == 2)
    {
        uint8_t i;
        int datarate = -1;
        for (i = 0; i < 8; i++)
        {
            if (mDot::DataRateStr(i).find(args[1].c_str()) != std::string::npos)
            {
                datarate = i;
                break;
            }
        }

        if (datarate < 0)
        {       
            if (_dot->getFrequencyBand() == mDot::FB_915)
                setErrorMessage("Invalid data rate, expects (7-10)");
            else
                setErrorMessage("Invalid data rate, expects (7-12)");
            return false;
        }   

        if (_dot->getFrequencyBand() == mDot::FB_915) {
            if (datarate < 2) {
                setErrorMessage("Invalid data rate, expects (7-10)");
                return false;
            }
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}
