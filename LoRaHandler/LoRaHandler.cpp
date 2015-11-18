#include "LoRaHandler.h"

#define signal (int32_t)0xA0

typedef enum {
    l_none = 0,
    l_ping,
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
    mDot::ping_response pr;
    mDot::rssi_stats rs;
    mDot::snr_stats ss;

    while (true) {
        e = Thread::signal_wait(signal);
        if (e.status == osEventSignal) {
            l->_status = LoRaHandler::busy;
            switch (cmd) {
                case l_ping:
                    l->_mutex.lock();
                    pr = l->_dot->ping();
                    l->_mutex.unlock();
                    if (pr.status == mDot::MDOT_OK) {
                        l->_ping.up = pr;
                        l->_mutex.lock();
                        rs = l->_dot->getRssiStats();
                        ss = l->_dot->getSnrStats();
                        l->_mutex.unlock();
                        l->_ping.down.rssi = rs.last;
                        l->_ping.down.snr = ss.last;
                        l->_status = LoRaHandler::ping_success;
                    } else {
                        l->_status = LoRaHandler::ping_failure;
                    }
                    osSignalSet(l->_main, loraSignal);
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
                    break;

                case l_join:
                    l->_mutex.lock();
                    ret = l->_dot->joinNetworkOnce();
                    l->_mutex.unlock();
                    if (ret == mDot::MDOT_OK) {
                        l->_status = LoRaHandler::join_success;
                    } else {
                        l->_status = LoRaHandler::join_failure;
                    }
                    osSignalSet(l->_main, loraSignal);
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
    _status(none)
{
    _ping.status = false;
}

bool LoRaHandler::setDataRate(uint8_t rate) {
    int32_t res;
    _mutex.lock();
    res = _dot->setTxDataRate(rate);
    _mutex.unlock();
    if (res == mDot::MDOT_OK)
        return true;

    return false;
}

bool LoRaHandler::setPower(uint32_t power) {
    int32_t res;
    _mutex.lock();
    res = _dot->setTxPower(power);
    _mutex.unlock();
    if (res == mDot::MDOT_OK)
        return true;

    return false;
}

bool LoRaHandler::ping() {
    return action(l_ping);
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

LoRaHandler::LoRaPing LoRaHandler::getPingResults() {
    LoRaPing ping;
    _mutex.lock();
    ping = _ping;
    _mutex.unlock();

    return ping;
}

uint32_t LoRaHandler::getNextTx() {
    uint32_t ms;
    _mutex.lock();
    ms = _dot->getNextTxMs();
    _mutex.unlock();

    return ms;
}

