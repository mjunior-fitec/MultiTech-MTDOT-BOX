#include "ctype.h"
#include "CommandTerminal.h"
#include "Command.h"
#include "MTSLog.h"
#include <cstdarg>
#include <deque>

const char CommandTerminal::banner[] = "\r\n\nMultiTech Systems LoRa XBee Module\r\n\n";
const char CommandTerminal::helpline[] = "Enter '?' for help\r\n";

const char CommandTerminal::newline[] = "\r\n";

// Command error text...
const char CommandTerminal::command_error[] = "Command not found!\r\n";

// Response texts...
const char CommandTerminal::help[] = "\r\nHelp\r\n";
const char CommandTerminal::cmd_error[] = "Invalid command\r\n";
const char CommandTerminal::done[] = "\r\nOK\r\n";
const char CommandTerminal::error[] = "\r\nERROR\r\n";

// Escape sequence...
const char CommandTerminal::escape_sequence[] = "+++";

mts::MTSSerial* CommandTerminal::_serialp = NULL;

void CommandTerminal::addCommand(Command* cmd) {
    _commands.push_back(cmd);
}

CommandTerminal::CommandTerminal(mts::MTSSerial& serial, mDot* dot)
:
  _serial(serial),
  _dot(dot),
  _mode(mDot::COMMAND_MODE),
  _idle_thread(idle, NULL, osPriorityLow),
  _sleep_standby(true),
  _xbee_on_sleep(XBEE_ON_SLEEP),
  _serial_up(false) {

    _serialp = &serial;

    addCommand(new CmdAttention(_dot));
    addCommand(new CmdIdentification(_dot, serial));
    addCommand(new CmdFactoryDefault(_dot));
    addCommand(new CmdSaveConfig(_dot));
    addCommand(new CmdDisplayConfig(_dot, serial));

    addCommand(new CmdFrequencySubBand(_dot, serial));
    addCommand(new CmdPublicNetwork(_dot, serial));
    addCommand(new CmdDeviceId(_dot, serial));

    addCommand(new CmdNetworkAddress(_dot, serial));
    addCommand(new CmdNetworkSessionKey(_dot, serial));
    addCommand(new CmdDataSessionKey(_dot, serial));
    addCommand(new CmdNetworkKey(_dot, serial));
    addCommand(new CmdNetworkId(_dot, serial));

    addCommand(new CmdNetworkJoinMode(_dot, serial));
    addCommand(new CmdTxDataRate(_dot, serial));
    addCommand(new CmdTxPower(_dot, serial));
}

void CommandTerminal::printHelp() {
    const char* name = NULL;
    const char* text = NULL;
    const char* desc = NULL;
    const char* tab = "\t";

    std::string header("Command");
    header.append(tab);
    header.append(tab);
    header.append("Name");
    header.append(tab);
    header.append(tab);
    header.append(tab);
    header.append("Description");

    write(newline);
    write(header.c_str());
    write(newline);
    write(newline);
    for (std::vector<Command*>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
        name = (*it)->name();
        text = (*it)->text();
        desc = (*it)->desc();
        write(text);
        if (strlen(text) < 8)
            write(tab);
        write(tab);
        write(name);
        if (strlen(name) < 8)
            write(tab);
        if (strlen(name) < 16)
            write(tab);
        write(tab);
        write(desc);
        write(newline);
    }

    write(newline);
}

bool CommandTerminal::writeable() {
    return _serial.writeable();
}

bool CommandTerminal::readable() {
    return _serial.readable();
}

char CommandTerminal::read() {
    char ch;
    _serial.read(&ch, 1);
    return ch;
}

void CommandTerminal::write(const char* message) {
    while (!writeable())
        ;
    _serial.write(message, strlen(message));
}

void CommandTerminal::writef(const char* format, ...) {
    char buff[256];

    va_list ap;
    va_start(ap, format);
    int size = vsnprintf(buff, 256, format, ap);
    while (!writeable())
        ;
    _serial.write(buff, size);
    va_end(ap);
}

void CommandTerminal::serial_loop() {
    Timer serial_read_timer;
    std::vector<uint8_t> serial_buffer;
    std::string escape_buffer;
    Timer escape_timer;
    int escape_delay = 100;
    uint8_t max_send_size;

    _serial_up = true;
    _xbee_on_sleep = GPIO_PIN_SET;

    if (_dot->getFrequencyBand() == mDot::FB_915)
        max_send_size = mDot::MaxLengths_915[_dot->getTxDataRate()];
    else
        max_send_size = mDot::MaxLengths_868[_dot->getTxDataRate()];

    logDebug("Awake\r\n");
    wakeup(_sleep_standby);

    char ch;

    if (readable()) {
        ch = read();
        serial_buffer.push_back(ch);

        if (escape_timer.read_ms() > escape_delay && ch == '+') {
            escape_buffer += ch;
            escape_timer.reset();
        } else {
            _serial_up = true;
            escape_buffer.clear();
        }

        if (escape_buffer.length() == 3 && escape_buffer.find(escape_sequence) == 0) {
            _mode = mDot::COMMAND_MODE;
            logDebug("Exit serial mode\r\n");
            escape_timer.stop();
            escape_buffer.clear();
            write(done);
            return;
        }
    }

    if (_serial_up) {
        serial_read_timer.start();
        uint32_t timeout = _dot->getWakeDelay();

        // wait for timeout or start of serial data
        while (!readable() && serial_read_timer.read_ms() < timeout) {
            osDelay(10);
        }

        serial_read_timer.reset();
        timeout = _dot->getWakeTimeout();
        while (_serial_up && serial_read_timer.read_ms() < timeout) {
            while (readable() && serial_buffer.size() < max_send_size) {
                serial_buffer.push_back(read());
                serial_read_timer.reset();
            }
        }
        serial_read_timer.stop(), serial_read_timer.reset();

        if (!serial_buffer.empty()) {
            _serial_up = false;
            _xbee_on_sleep = GPIO_PIN_RESET;
            if (!_dot->getIsTransmitting()) {
                std::vector<uint8_t> recv_buffer;
                logDebug("Received serial data, sending out radio.\r\n");

                if (_dot->send(serial_buffer) != mDot::MDOT_OK)
                    logDebug("Send failed.\r\n");

                if (_dot->recv(recv_buffer))
                    _serial.writef("%s\r\n", formatPacketData(recv_buffer, _dot->getRxOutput()).c_str());
            } else {
                logDebug("Radio is busy, cannot send.\r\n");
            }

            serial_buffer.clear();
        } else {
            logDebug("No data received from serial to send.\r\n");
        }
        _serial_up = false;
    }
    sleep(_sleep_standby);
}

bool CommandTerminal::autoJoinCheck() {

    std::string escape_buffer;
    char ch;
    int sleep = 1000;
    int escape_timeout = 1000;
    Timer tmr;
    Timer escape_tmr;
    int cnt = 0;

    while (!_dot->getNetworkJoinStatus()) {
        wakeup(_sleep_standby);
        write("\r\nJoining network... ");

        // wait one second for possible escape
        tmr.reset();
        tmr.start();
        escape_tmr.reset();
        escape_tmr.start();
        while (tmr.read_ms() < 1000) {
            if (_serial.readable()) {
                _serial.read(&ch, 1);
                escape_buffer += ch;
            }

            if (escape_buffer.find(escape_sequence) != std::string::npos) {
                _mode = mDot::COMMAND_MODE;
                write("Join Canceled\r\n");
                write(done);
                return true;
            }

            if (escape_tmr.read_ms() > escape_timeout)
                escape_buffer.clear();
        }

        if (_dot->joinNetworkOnce() == mDot::MDOT_OK) {
            write("Network Joined\r\n");
            write(done);
            return false;
        }

        write("Network Join failed\r\n");
        write(error);

        if (cnt++ > _dot->getJoinRetries()) {
            cnt = 0;

            if (_dot->getFrequencyBand() == mDot::FB_915) {
                uint8_t band = ((_dot->getFrequencySubBand()) % 8) + 1;
                logDebug("Join retries exhausted, switching to sub band %u\r\n", band);
                _dot->setFrequencySubBand(band);
            }

            if (sleep < 60 * 60 * 1000)
                sleep *= 2;
        }

        tmr.reset();
        tmr.start();
        escape_tmr.reset();
        escape_tmr.start();
        while (tmr.read_ms() < sleep) {
            if (_serial.readable()) {
                _serial.read(&ch, 1);
                escape_buffer += ch;
            }

            if (escape_buffer.find(escape_sequence) != std::string::npos) {
                _mode = mDot::COMMAND_MODE;
                return true;
            }

            if (escape_tmr.read_ms() > escape_timeout)
                escape_buffer.clear();
        }

    }

    return false;
}

void CommandTerminal::start() {

    wakeup(_sleep_standby);

    char ch;
    bool running = true;
    bool echo = _dot->getEcho();
    std::string command;
    std::deque<std::string> history;
    int history_index = -1;
    std::vector<std::string> args;

    if (_dot->getStartUpMode() == mDot::SERIAL_MODE) {
        command = "AT+SD\n";

        std::string escape_buffer;
        char ch;

        int escape_timeout = 1000;
        Timer tmr;
        Timer escape_tmr;

        // wait one second for possible escape
        tmr.reset();
        tmr.start();
        escape_tmr.reset();
        escape_tmr.start();
        while (tmr.read_ms() < escape_timeout) {
            if (_serial.readable()) {
                _serial.read(&ch, 1);
                escape_buffer += ch;
            }

            if (escape_buffer.find(escape_sequence) != std::string::npos) {
                _mode = mDot::COMMAND_MODE;
                command.clear();
                break;
            }

            if (escape_tmr.read_ms() > escape_timeout)
                escape_buffer.clear();

            osDelay(1);
        }

    }

    bool join_canceled = false;

    //Run terminal session
    while (running) {
        if (!join_canceled && _dot->getJoinMode() == mDot::AUTO_OTA) {
            join_canceled = autoJoinCheck();
            if (join_canceled)
                command.clear();
        }

        if (_dot->getJoinMode() != mDot::AUTO_OTA || (!join_canceled && _dot->getJoinMode() == mDot::AUTO_OTA)) {
            switch (_mode) {
                case mDot::SERIAL_MODE:
                    // signal wakeup, read serial and output to radio
                    serial_loop();
                    continue;
                    break;
                default:
                    break;
            }
        }

        ch = '\0';

        // read characters
        if (readable()) {
            ch = read();

            if (ch == '\b' || ch == 0x7f) {
                if (!command.empty()) {
                    writef("\b \b");
                    command.erase(command.size() - 1);
                }
                continue;
            } else if (ch == 0x1b || ch == 0x09) {
                osDelay(20);
                // catch escape sequence, or tab
                char ch1, ch2;

                if (readable()) {
                    ch1 = read();
                    if (readable())
                        ch2 = read();

                    if (ch1 == 0x5b && ch2 == 0x41) {
                        // up key
                        for (int i = 0; i < command.size()+1; i++) {
                            writef("\b \b");
                        }
                        if (history.size() > 0) {
                            if (++history_index >= history.size() - 1)
                                history_index = history.size() - 1;

                            command = history[history_index];
                            writef("%s", history[history_index].c_str());
                        } else {
                            command.clear();
                        }
                    } else if (ch1 == 0x5b && ch2 == 0x42) {

                        // down key
                        for (int i = 0; i < command.size()+1; i++) {
                            writef("\b \b");
                        }

                        if (--history_index < 0) {
                            history_index = -1;
                            command.clear();
                        } else {
                            command = history[history_index];
                            writef("%s", history[history_index].c_str());
                        }
                    }
                }
                while (readable()) read();
                continue;
            } else {
                command += ch;
            }

            // echo chars if enabled
            if (echo && !(ch == '\r' || ch == '\n'))
                writef("%c", ch);
        }

        // look for end of command line
        if (command.find("\n") != std::string::npos || command.find("\r") != std::string::npos) {
            // remove new line or cr character
            command.erase(command.size() - 1);
            write("\r"); // match standard modem output
            write(newline);
        } else {
            continue;
        }

        // trim whitespace from command
        mts::Text::trim(command, "\r\n\t ");

        if (command.size() < 1) {
            command.clear();
            continue;
        }

        // parse command and args
        args.clear();

        // find first '=' character
        size_t delim_index = command.find("=");
        if (delim_index != std::string::npos) {
            args.push_back(command.substr(0, delim_index));
        } else {
            // find first ' ' character
            delim_index = command.find(" ");
            if (delim_index != std::string::npos) {
                args.push_back(command.substr(0, delim_index));
            } else {
                args.push_back(command);
            }
        }

        if (delim_index != std::string::npos) {
            std::vector<std::string> params = mts::Text::split(command.substr(delim_index + 1), ",");
            args.insert(args.end(), params.begin(), params.end());
        }

        args[0] = mts::Text::toUpper(args[0]);

        // print help
        if ((args[0].find("?") == 0 || args[0].find("HELP") == 0) && args.size() == 1) {
            printHelp();
            command.clear();
        } else if (args[0].find("ATE0") == 0 && args[0].length() == 4) {
            _dot->setEcho(false);
            write(done);
            echo = _dot->getEcho();
        } else if (args[0].find("ATE1") == 0 && args[0].length() == 4) {
            _dot->setEcho(true);
            write(done);
            echo = _dot->getEcho();
        } else if (args[0].find("ATV0") == 0 && args[0].length() == 4) {
            _dot->setVerbose(false);
            write(done);
        } else if (args[0].find("ATV1") == 0 && args[0].length() == 4) {
            _dot->setVerbose(true);
            write(done);
        } else if (args[0] == "AT+SD") {
            logDebug("Enter Serial Mode\r\n");
            _mode = mDot::SERIAL_MODE;
        } else if (args[0] == "AT+SLEEP") {
            if (args.size() > 1 && (args[1] != "?")) {
                write("Invalid argument\r\n");
                write(error);
            } else {
                if (args.size() > 1 && args[1] == "?") {
                    write("AT+SLEEP: NONE\r\n");
                    write(done);
                } else {
                    _sleep_standby = !(args.size() > 1 && args[1] == "1");
                    this->sleep(_sleep_standby);
                    wait(0.1);
                    write(done);
                }
            }
        } else {
            bool found = false;
            bool query = false;

            std::string lookfor = args[0];

            // per command help
            if ((args[0].find("?") == 0 || args[0].find("HELP") == 0))
                lookfor = mts::Text::toUpper(args[1]);

            // trim off any trailing '?' and mark as a query command
            if (args[0].rfind("?") == args[0].length() - 1) {
                query = true;
                lookfor = args[0].substr(0, args[0].length() - 1);
            }

            // search for command
            for (std::vector<Command*>::iterator it = _commands.begin(); it != _commands.end() && !found; ++it) {
                Command* cmd = *it;

                // match CMD or CMD? syntax if command is queryable
                if (lookfor == cmd->text() && (!query || (query && cmd->queryable()))) {
                    found = true;
                    if (args[0] == "HELP") {
                        writef("%s%s", cmd->help(), newline);
                        write(done);
                    }

                    else if (args.size() > 1 && args[1] == "?") {
                        writef("%s%s", cmd->usage().c_str(), newline);
                        write(done);
                    } else if (!cmd->verify(args)) {
                        writef("%s%s", cmd->errorMessage().c_str(), newline);
                        writef("%s", error);
                    } else {
                        if (cmd->action(args) == 0) {
                            writef("%s", done);
                        } else {
                            writef("%s%s", cmd->errorMessage().c_str(), newline);
                            writef("%s", error);
                        }
                    }
                }
            }

            if (!found) {
                writef("%s", command_error);
                writef("%s", error);
            }
        }

        if (history.size() == 0 || history.front() != command)
            history.push_front(command);
        history_index = -1;
        command.clear();

        while (history.size() > 10)
            history.pop_back();

    }
}

std::string CommandTerminal::formatPacketData(const std::vector<uint8_t>& data, const uint8_t& format) {
    if (format == mDot::HEXADECIMAL)
        return mts::Text::bin2hexString(data);
    else
        return std::string(data.begin(), data.end());
}

void CommandTerminal::sleep(bool standby) {
    _serial_up = false;
    _xbee_on_sleep = GPIO_PIN_RESET;

    _serial.rxClear();
    _serial.txClear();

    _dot->sleep(_dot->getWakeInterval(), _dot->getWakeMode(), standby);
}

bool CommandTerminal::waitForEscape(int timeout, mDot* dot, WaitType wait) {
    Timer timer;
    Timer escape_timer;
    std::string escape_buffer;
    int escape_timeout = 1000;

    timer.start();
    while (timer.read_ms() < timeout) {

        if (dot != NULL) {
            if (wait == WAIT_SEND && (!dot->getIsTransmitting())) {
                return false;
            }
        }

        if (_serialp != NULL && _serialp->readable()) {
            if (escape_buffer == "")
                escape_timer.start();
            char ch;
            _serialp->read(&ch, 1);
            escape_buffer += ch;
            if (escape_buffer == CommandTerminal::escape_sequence)
                return true;
        }
        if (escape_timer.read_ms() > escape_timeout) {
            escape_buffer = "";
            escape_timer.stop(), escape_timer.reset();
        }

        osDelay(10);
    }

    return false;
}

void CommandTerminal::wakeup(bool standby) {

}
