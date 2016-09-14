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
#include "MTSText.h"
#include "MTSLog.h"

CmdTxDataRate::CmdTxDataRate(mDot* dot, mts::MTSSerial& serial)
:
  Command(dot, "Tx Data Rate", "AT+TXDR", "Set the Tx data rate for all channels"),
  _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());

    if (_dot->getFrequencyBand() == mDot::FB_915)
        _usage = "(7-10|DR0-DR4|DR8-DR13)";
    else
        _usage = "(7-12|DR0-DR7)";
    _queryable = true;
}

uint32_t CmdTxDataRate::action(std::vector<std::string> args) {
    if (args.size() == 1)
        {
        if (_dot->getVerbose())
            _serial.writef("Tx Data Rate: ");

        _serial.writef("DR%d - %s\r\n", _dot->getTxDataRate(), _dot->getDateRateDetails(_dot->getTxDataRate()).c_str());
    } else if (args.size() == 2) {
        std::string dr = mts::Text::toUpper(args[1]);
        int32_t code;
        int datarate = -1;
        uint8_t i;

        int res = sscanf(dr.c_str(), "%d", &datarate);

        if (res == 0) {
            for (i = 0; i < 24; i++) {
                if (mDot::DataRateStr(i).find(dr) != std::string::npos) {
                    datarate = i;
                    break;
                }
            }
        } else {
            if (datarate > 6) {
                // Convert SF to DR
                if (_dot->getFrequencyBand() == mDot::FB_915) {
                    datarate = 10 - datarate;
                } else {
                    datarate = 12 - datarate;
                }
            }
        }

        if ((code = _dot->setTxDataRate(datarate)) != mDot::MDOT_OK) {
            setErrorMessage(_dot->getLastError());
            return 1;
        }
    }

    return 0;
}

bool CmdTxDataRate::verify(std::vector<std::string> args) {
    if (args.size() == 1)
        return true;

    if (args.size() == 2) {
        std::string dr = mts::Text::toUpper(args[1]);

        uint8_t i;
        int datarate = -1;

        int res = sscanf(dr.c_str(), "%d", &datarate);

        if (res == 0) {
            for (i = 0; i < 24; i++) {

                if (mDot::DataRateStr(i).find(dr) != std::string::npos) {
                    uint8_t _dr = i;

                    if (_dr > 15) {
                        _dr = 12 + (mDot::SF_12 - _dr);

                        if (_dot->getFrequencyBand() == mDot::FB_868) {
                            if (_dr == mDot::SF_7H) {
                                _dr =  mDot::DR6;
                            } else if (_dr ==  mDot::SF_FSK) {
                                _dr =  mDot::DR7;
                            } else {
                                _dr = 12 - _dr;
                            }
                        } else {

                            _dr = 10 - _dr;
                        }
                    }

                    datarate = _dr;
                    break;
                }
            }
        } else {
            if (_dot->getFrequencyBand() == mDot::FB_915 && datarate > 10) {
                datarate = -1;
            } else if (_dot->getFrequencyBand() == mDot::FB_868 && datarate > 12) {
                datarate = -1;
            } else if (datarate > 6) {
                // Convert SF to DR
                if (_dot->getFrequencyBand() == mDot::FB_915) {
                    datarate = 10 - datarate;
                } else {
                    datarate = 12 - datarate;
                }
            }
        }

        if (_dot->getFrequencyBand() == mDot::FB_915) {
            // DR8-13 used for P2P modes
            if (datarate < 0 || datarate > 13 || (datarate >= 5 && datarate <= 7)) {
                setErrorMessage("Invalid data rate, expects (7-10|DR0-DR4|DR8-DR13)");
                return false;
            }
        } else {
            if (datarate < 0 || datarate > 7) {
                setErrorMessage("Invalid data rate, expects (7-12|DR0-DR7)");
                return false;
            }
        }

        return true;
    }

    setErrorMessage("Invalid arguments");
    return false;
}
