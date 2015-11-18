#include "CmdDisplayConfig.h"

CmdDisplayConfig::CmdDisplayConfig(mDot* dot, mts::MTSSerial& serial)
:
  Command(dot, "Display Settings", "AT&V", "Displays current settings and status"),
  _serial(serial) {
    _help = std::string(text()) + ": " + std::string(desc());
    _usage = "TABLE";
}

uint32_t CmdDisplayConfig::action(std::vector<std::string> args) {
    _serial.writef("Device ID:\t\t");
    _serial.writef("%s\r\n", mts::Text::bin2hexString(_dot->getDeviceId(), ":").c_str());

    _serial.writef("Frequency Band:\t\t%s\r\n", mDot::FrequencyBandStr(_dot->getFrequencyBand()).c_str());
    _serial.writef("Frequency Sub Band:\t%u\r\n", _dot->getFrequencySubBand());

    _serial.writef("Public Network:\t\t%s\r\n", _dot->getPublicNetwork() ? "on" : "off");
    _serial.writef("Start Up Mode:\t\t%s\r\n", mDot::ModeStr(_dot->getStartUpMode()).c_str());

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

    _serial.writef("Network Join Retries:\t%u\r\n", _dot->getJoinRetries());

    _serial.writef("Join Byte Order:\t%s\r\n", _dot->getJoinByteOrder() ? "MSB" : "LSB");

    _serial.writef("Link Check Threshold:\t");
    if (_dot->getLinkCheckThreshold() == 0) {
        _serial.writef("off\r\n");
    } else {
        _serial.writef("%lu\r\n", _dot->getLinkCheckThreshold());
    }

    _serial.writef("Link Check Count:\t");
    if (_dot->getLinkCheckCount() == 0) {
        _serial.writef("off\r\n");
    } else {
        _serial.writef("%lu packets\r\n", _dot->getLinkCheckCount());
    }

    _serial.writef("Error Correction:\t");
    if (_dot->getFec() == 0) {
        _serial.writef("off\r\n");
    } else {
        _serial.writef("%u bytes\r\n", _dot->getFec());
    }

    _serial.writef("ACK Retries:\t\t");
    if (_dot->getAck() == 0) {
        _serial.writef("off\r\n");
    } else {
        _serial.writef("%u\r\n", _dot->getAck());
    }

    _serial.writef("Encryption:\t\t%s\r\n", _dot->getAesEncryption() ? "on" : "off");
    _serial.writef("CRC:\t\t\t%s\r\n", _dot->getCrc() ? "on" : "off");
    _serial.writef("Adaptive Data Rate:\t%s\r\n", _dot->getAdr() ? "on" : "off");
    _serial.writef("Command Echo:\t\t%s\r\n", _dot->getEcho() ? "on" : "off");
    _serial.writef("Verbose Response:\t%s\r\n", _dot->getVerbose() ? "on" : "off");

    _serial.writef("Tx Frequency:\t\t%lu\r\n", _dot->getTxFrequency());
    _serial.writef("Tx Data Rate:\t\t%s\r\n", mDot::DataRateStr(_dot->getTxDataRate()).c_str());
    _serial.writef("Tx Power:\t\t%u\r\n", _dot->getTxPower());
    _serial.writef("Tx Wait:\t\t%s\r\n", _dot->getTxWait() ? "on" : "off");

    _serial.writef("Tx Inverted Signal:\t%s\r\n", _dot->getTxInverted() ? "on" : "off");

    _serial.writef("Rx Frequency:\t\t%lu\r\n", _dot->getRxFrequency());
    _serial.writef("Rx Data Rate:\t\t%s\r\n", mDot::DataRateStr(_dot->getRxDataRate()).c_str());
    _serial.writef("Rx Inverted Signal:\t%s\r\n", _dot->getRxInverted() ? "on" : "off");

    _serial.writef("Rx Output Style:\t%s\r\n", mDot::RxOutputStr(_dot->getRxOutput()).c_str());

    _serial.writef("Debug Baud Rate:\t%lu\r\n", _dot->getDebugBaud());
    _serial.writef("Serial Baud Rate:\t%lu\r\n", _dot->getBaud());

    _serial.writef("Wake Mode:\t\t%s\r\n", _dot->getWakeMode() == 0 ? "INTERVAL" : "INTERRUPT");
    _serial.writef("Wake Interval:\t\t%lu s\r\n", _dot->getWakeInterval());
    _serial.writef("Wake Delay:\t\t%lu ms\r\n", _dot->getWakeDelay());
    _serial.writef("Wake Timeout:\t\t%u ms\r\n", _dot->getWakeTimeout());

    //_serial.writef("Wake Pin:\t\t%s\r\n", mDot::pinName2Str(_dot->getWakePin()).c_str());

    _serial.writef("Log Level:\t\t%ld\r\n", _dot->getLogLevel());

    return 0;
}

