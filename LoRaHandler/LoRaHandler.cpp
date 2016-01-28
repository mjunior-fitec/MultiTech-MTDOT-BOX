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

#include "LoRaHandler.h"

#define signal (int32_t)0xA0

typedef enum {
    l_none = 0,
    l_link_check,
    l_send,
    l_join
} InternalLoRa;

uint8_t cmd = l_none;
std::vector<uint8_t> send_data;

void l_worker(void const* argument) {
    LoRaHandler* l = (LoRaHandler*)argument;
    osEvent e;

    l->_dot = mDot::getInstance();
    int32_t ret;
    mDot::link_check lc;
    mDot::rssi_stats rs;
    mDot::snr_stats ss;

    while (true) {
        e = Thread::signal_wait(signal);
        if (e.status == osEventSignal) {
            l->_status = LoRaHandler::busy;
            l->_tick.attach(l, &LoRaHandler::blinker, 0.05);
            switch (cmd) {
                case l_link_check:
                    l->_mutex.lock();
                    lc = l->_dot->networkLinkCheck();
                    l->_mutex.unlock();
                    if (lc.status) {
                        l->_link.up = lc;
                        l->_mutex.lock();
                        rs = l->_dot->getRssiStats();
                        ss = l->_dot->getSnrStats();
                        l->_mutex.unlock();
                        l->_link.down.rssi = rs.last;
                        l->_link.down.snr = ss.last;
                        l->_status = LoRaHandler::link_check_success;
                    } else {
                        l->_status = LoRaHandler::link_check_failure;
                    }
                    osSignalSet(l->_main, loraSignal);
                    l->_tick.detach();
                    l->_activity_led = LoRaHandler::green;
                    break;
                
                case l_send:
                    l->_mutex.lock();
                    ret = l->_dot->send(send_data);
                    l->_mutex.unlock();
                    if (ret == mDot::MDOT_OK)
                        l->_status = LoRaHandler::send_success;
                    else
                        l->_status = LoRaHandler::send_failure;
                    osSignalSet(l->_main, loraSignal);
                    l->_tick.detach();
                    l->_activity_led = LoRaHandler::green;
                    break;

                case l_join:
                    l->_mutex.lock();
                    ret = l->_dot->joinNetworkOnce();
                    l->_join_attempts++;
                    l->_mutex.unlock();
                    if (ret == mDot::MDOT_OK) {
                        l->_status = LoRaHandler::join_success;
                    } else {
                        l->_status = LoRaHandler::join_failure;
                    }
                    osSignalSet(l->_main, loraSignal);
                    l->_tick.detach();
                    l->_activity_led = LoRaHandler::green;
                    break;

                default:
                    l->_status = LoRaHandler::none;
                    break;
            }
        }
    }
}

LoRaHandler::LoRaHandler(osThreadId main)
  : _main(main),
    _thread(l_worker, (void*)this),
    _status(none),
    _join_attempts(1),
    _activity_led(XBEE_DIO1, PIN_OUTPUT, PullNone, red)
{
    _link.status = false;
    _activity_led = red;
}

bool LoRaHandler::linkCheck() {
    return action(l_link_check);
}

bool LoRaHandler::send(std::vector<uint8_t> data) {
    send_data = data;
    return action(l_send);
}

bool LoRaHandler::join() {
        return action(l_join);
}

bool LoRaHandler::action(uint8_t c) {
    if (_status != busy) {
        cmd = c;
        _thread.signal_set(signal);
        _thread.signal_clr(signal);
        return true;
    }

    return false;
}

LoRaHandler::LoRaStatus LoRaHandler::getStatus() {
    LoRaStatus status;
    _mutex.lock();
    status = _status;
    _mutex.unlock();

    return status;
}

LoRaHandler::LoRaLink LoRaHandler::getLinkCheckResults() {
    LoRaLink link;
    _mutex.lock();
    link = _link;
    _mutex.unlock();

    return link;
}

uint32_t LoRaHandler::getJoinAttempts() {
    uint32_t val;

    _mutex.lock();
    val = _join_attempts;
    _mutex.unlock();

    return val;
}

void LoRaHandler::resetJoinAttempts() {
    _mutex.lock();
    _join_attempts = 1;
    _mutex.unlock();
}

void LoRaHandler::blinker() {
    _activity_led = !_activity_led;
}

void LoRaHandler::resetActivityLed() {
    _activity_led = red;
}

