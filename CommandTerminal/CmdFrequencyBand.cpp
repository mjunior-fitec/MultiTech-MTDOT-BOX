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

#include "CmdFrequencyBand.h"

CmdFrequencyBand::CmdFrequencyBand(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Frequency Band", "AT+FREQ", "Configured Frequency Band 'EU868', 'AU915' or 'US915'"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "(EU868,AU915,US915)";
    _queryable = true;
}

uint32_t CmdFrequencyBand::action(std::vector<std::string> args)
{
    if (args.size() == 1)
    {
        if (_dot->getVerbose())
            _serial.writef("Frequency Band: ");

        _serial.writef("%s\r\n", mDot::FrequencyBandStr(_dot->getFrequencyBand()).c_str());
    }

#ifdef DEBUG_MAC
    else if (args.size() == 2)
    {
        int32_t code;

        std::string text = mts::Text::toUpper(args[1]);

        uint8_t band = mDot::FB_US915;


        if (mDot::FrequencyBandStr(mDot::FB_EU868).find(text) != std::string::npos) {
            band = mDot::FB_EU868;
        }

        if (mDot::FrequencyBandStr(mDot::FB_AU915).find(text) != std::string::npos) {
            band = mDot::FB_AU915;
        }

        if (mDot::FrequencyBandStr(mDot::FB_US915).find(text) != std::string::npos) {
            band = mDot::FB_US915;
        }

        if ((code = _dot->setFrequencyBand(band)) != mDot::MDOT_OK) {
            std::string error = mDot::getReturnCodeString(code) + " - " + _dot->getLastError();
            setErrorMessage(error);
            return 1;
        }
    }
#endif

    return 0;
}

bool CmdFrequencyBand::verify(std::vector<std::string> args)
{
    if (args.size() == 1)
        return true;

#ifdef DEBUG_MAC
    if (args.size() == 2)
    {
        std::string band = mts::Text::toUpper(args[1]);

        if (mDot::FrequencyBandStr(mDot::FB_EU868).find(band) == std::string::npos &&
            mDot::FrequencyBandStr(mDot::FB_US915).find(band) == std::string::npos &&
            mDot::FrequencyBandStr(mDot::FB_AU915).find(band) == std::string::npos)
        {
            setErrorMessage("Invalid parameter, expects (EU868,AU915,US915)");
            return false;
        }

        return true;
    }
#endif

    setErrorMessage("Invalid arguments");
    return false;
}
