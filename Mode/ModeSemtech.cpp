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

#include "ModeSemtech.h"
#include "MTSLog.h"
#include "rtos.h"
#include "mbed.h"
#include <sstream>
//this is exactly what it sounds like
#define TWO_TO_THE_THIRTY_ONE_MINUS_ONE 2147483647
#define PACKETSIZE 11




ModeSemtech::ModeSemtech(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors)
    : Mode(lcd, buttons, dot, lora, gps, sensors),
      _sem(lcd,_band),
      _help(lcd),
      _sem_join(lcd,_band)
{}

string ModeSemtech::intToString(int num)
{
    return static_cast<ostringstream*>(&(ostringstream() << num))->str();
}

void ModeSemtech::init()
{
    //resets all parameters when re-entering mode 
    _interval = 5;
    _padding = 0;
    _power = 20;
    _band = _dot->getFrequencyBand(); 
    _parameter = POWER;
    _drAll=false;
    _link_check = false;
    _GPS = false;
    _sub_band = _dot->getFrequencySubBand();
    _data_rate = mDot::DR4;
    _max_padding = _dot->getMaxPacketLength()-PACKETSIZE;
    _Sw2 = "Power";
    _Sw1 = intToString(_power);
    _help.display();
    osDelay(2000);
    if(_band==mDot::FB_868){
         _sem.display(_link_check, _snr, _rssi, _power, _sub_band, _padding, _data_rate);
         _sem.initial();
         _state = PARAMETERS;
         _send_timer.start();
         }
    else { _state = BAND_CHANGE;
          changeBand();
         }
}

void ModeSemtech::drIncrement()
{
    _data_rate++;
    if (_data_rate > mDot::DR5) {
        _drAll = true;
        _data_rate=0;
    }
    _dot->setTxDataRate(_data_rate);
    logInfo("new data rate %s, POWER %lu", mDot::DataRateStr(_data_rate).c_str(), _power);
    _max_padding = _dot->getMaxPacketLength()-PACKETSIZE;
}

void ModeSemtech::changeDataRate()
{
    if(_drAll) {
       if(_band == mDot::FB_868) _data_rate = -1;
       else _data_rate = mDot::DR1;
        _drAll = false;
    }
    drIncrement();
}

void ModeSemtech::changeParameter()
{
    _parameter++;
    if(_band==mDot::FB_868&&_parameter==FSB)_parameter++;
    if(_parameter>INTERVAL)_parameter=0;
    switch(_parameter) {
        case DATA_RATE:
            _Sw2 = "Data Rate";
            _Sw1 = intToString(_data_rate);
            if(_drAll)_Sw1 = "All";
            break;
        case FSB:
            _Sw2 = "FSB";
            _Sw1 = intToString(_sub_band);
            break;
        case PADDING:
            _Sw2 = "Padding";
            _Sw1 = intToString(_padding);
            break;
        case POWER:
            _Sw2 = "Power";
            _Sw1 = intToString(_power);
            break;
        case INTERVAL:
            _Sw2 = "Interval";
            _Sw1 = intToString(_interval);
            break;
        default:
            break;
    }
}

void ModeSemtech::editParameter()
{
    switch(_parameter) {
        case POWER:
            if(_power<20)_power+=3;
            else _power = 2;
            _Sw1 = intToString(_power);
            _dot->setTxPower(_power);
            break;
            
        case DATA_RATE:
            changeDataRate();
            _Sw1 = intToString(_data_rate);
            if(_drAll)_Sw1="All";
            break;

        case FSB:
            _send_timer.stop();
            _send_timer.reset();
            _state = BAND_CHANGE;
            _dot->resetNetworkSession();
            _lora->resetActivityLed();
            changeBand();
            break;

        case PADDING:
            if(_padding<_max_padding)_padding +=10 - (_padding%10);
            else _padding = 0;
            if(_padding>_max_padding)_padding = _max_padding;
            _Sw1 = intToString(_padding);
            break;

        default:
            if(_interval<60)_interval += 5;
            else _interval = 5;
            _Sw1 = intToString(_interval);
            break;
    }
}

void ModeSemtech::formatData()
{
    _send_data.clear();
    uint32_t lat = 0;
    uint32_t lng = 0;
    _temp_C+=0.5;

    if(_GPS) {
        if(_latitude.degrees<0)  lat = ~(int)((_latitude.degrees - ((double)_latitude.minutes/60.0) - ((double)_latitude.seconds/600000.0))*((double)-TWO_TO_THE_THIRTY_ONE_MINUS_ONE/90.0)+1.5);
        else                     lat =  (int)((_latitude.degrees + ((double)_latitude.minutes/60.0) + ((double)_latitude.seconds/600000.0))*((double)TWO_TO_THE_THIRTY_ONE_MINUS_ONE/90.0)+0.5);
        if(_longitude.degrees<0) lng = ~(int)((_longitude.degrees - ((double)_longitude.minutes/60.0) - ((double)_longitude.seconds/600000.0))*((double)-TWO_TO_THE_THIRTY_ONE_MINUS_ONE/180.0)+1.5);
        else                     lng =  (int)((_longitude.degrees + ((double)_longitude.minutes/60.0) + ((double)_longitude.seconds/600000.0))*((double)TWO_TO_THE_THIRTY_ONE_MINUS_ONE/180.0)+0.5);
    }
    _send_data.push_back(0);
    _send_data.push_back((int8_t) _temp_C);
    _send_data.push_back(0);
    for(int i=24; i>=0; i-=8)_send_data.push_back((lat>>i)&0xFF);
    for(int i=24; i>=0; i-=8)_send_data.push_back((lng>>i)&0xFF);
    for(int i=0; i<(_padding>_max_padding ? _max_padding : _padding); i++) _send_data.push_back(0);
}

void ModeSemtech::setBand()
{
    _sub_band++;
    if(_sub_band > mDot::FSB_8) _sub_band = mDot::FSB_ALL;
    _dot->setFrequencySubBand(_sub_band);
}

void ModeSemtech::changeBand()
{
    _sem_join.display();
    _sem_join.updatePower(_power);
    _sem_join.updateFsb(_sub_band);
    _sem_join.updateId(_dot->getNetworkName());
    _sem_join.updateRate(intToString(_data_rate));
    _sem_join.updateKey(_dot->getNetworkPassphrase());
}

void ModeSemtech::updateScreen()
{
    _temp_C = _sensors->getTemp(SensorHandler::CELSIUS);
    if(_gps->getLockStatus()&& _gps_available) {
        _GPS = true;
        _latitude = _gps->getLatitude();
        _longitude = _gps->getLongitude();
        _time = _gps->getTimestamp();
    } else _GPS = false;
    _sem.updateStats( _GPS, _longitude, _latitude, _time, _temp_C);
    _sem.updateSw1(_Sw1,_Sw2,_data_rate,_power,_padding);
    _sem.updateSw2(_Sw2);
}

void ModeSemtech::send()
{   
    _state = SENDING;
    _send_timer.stop();
    if(_band==mDot::FB_868) {
        while(_dot->getNextTxMs()>0) {
            _sem.updateNextCh((int)(_dot->getNextTxMs()/1000));
            osDelay(250);
        }
    }
    formatData();
    _sem.sending();
    _send_timer.reset();
    _send_timer.start();
    _lora->send(_send_data);
    osDelay(500);
}

bool ModeSemtech::start()
{
    init();
    _button_timer.start();
    ButtonHandler::ButtonEvent be;
    osSignalClear(_main_id, buttonSignal | loraSignal);
    if(_band==mDot::FB_915)_join = new ModeJoin(_lcd, _buttons, _dot, _lora, _gps, _sensors);
    
    while (true) {
        if(_state==PARAMETERS)updateScreen();

        osEvent e = Thread::signal_wait(0, 250);
        if (e.status == osEventSignal) {
            if (e.value.signals & buttonSignal) {
                _button_timer.reset();
                be = _buttons->getButtonEvent();

                switch(be) {
                    case ButtonHandler::sw1_press:
                        switch(_state) {
                            case BAND_CHANGE:
                                setBand();
                                changeBand();
                                break;

                            case SENDING:
                                break;

                            case PARAMETERS:
                                editParameter();
                                break;
                        }
                        break;
                    case ButtonHandler::sw2_press:
                        switch(_state) {
                            case BAND_CHANGE:
                                if(_join->start()){
                                    _state = PARAMETERS;
                                    _send_timer.start();
                                    _sem.display(_link_check, _snr, _rssi, _power, _sub_band, _padding, DATA_RATE);
                                    _dot->setTxDataRate(_data_rate);
                                } else changeBand();
                                break;

                            case SENDING:
                                break;

                            case PARAMETERS:
                                changeParameter();
                                break;
                        }
                        break;
                    case ButtonHandler::sw1_hold:
                        _send_timer.stop();
                        _send_timer.reset();
                        return true;

                    default:
                        break;
                }

            }
        }
        if (e.value.signals & loraSignal) {
            _ls = _lora->getStatus();
            switch (_ls) {

                case LoRaHandler::send_success:
                    _sem.sendResult("Send Sucess!");
                    _link_check = true;
                    _snr = _dot->getSnrStats();
                    _rssi = _dot->getRssiStats();
                    osDelay(500);
                    _button_timer.reset();
                    _state = PARAMETERS;
                    _sem.display(_link_check, _snr, _rssi, _power, _sub_band, _padding, _data_rate);
                    if(_drAll)drIncrement();
                    break;

                case LoRaHandler::send_failure:
                    _sem.sendResult("Send Failed.");
                    _link_check = false;
                    osDelay(500);
                    _button_timer.reset();
                    _state = PARAMETERS;
                    _sem.display(_link_check, _snr, _rssi, _power, _sub_band, _padding, _data_rate);
                    if(_drAll)drIncrement();
                    break;

                default:
                    break;
            }
       
        }

        if(_send_timer.read_ms()>_interval*1000&&_button_timer.read_ms()>3000) send();

    }
}


