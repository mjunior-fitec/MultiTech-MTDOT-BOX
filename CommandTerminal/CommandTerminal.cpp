#include "ctype.h"
#include "CommandTerminal.h"
#include "Command.h"
#include "MTSLog.h"
#include "ButtonHandler.h"
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

CommandTerminal::CommandTerminal(mts::MTSSerial& serial, mDot* dot, ButtonHandler* buttons)
:
  _serial(serial),
  _dot(dot),
  _mode(mDot::COMMAND_MODE),
  _idle_thread(idle, NULL, osPriorityLow),
  _serial_up(false),
  _buttons(buttons) {

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

    addCommand(new CmdMinimumSize(_dot, serial));
    addCommand(new CmdMaximumSize(_dot, serial));
    addCommand(new CmdMinimumPower(_dot, serial));
    addCommand(new CmdMaximumPower(_dot, serial));
    addCommand(new CmdData(_dot, serial));
    addCommand(new CmdGetSurveyDataFile(_dot, serial));
    addCommand(new CmdDeleteSurveyDataFile(_dot, serial));
    addCommand(new CmdExit(_dot, serial));
    
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

bool CommandTerminal::start() {
    char ch;
    bool running = true;
    bool echo = _dot->getEcho();
    std::string command;
    std::deque<std::string> history;
    int history_index = -1;
    std::vector<std::string> args;

    //Run terminal session
    while (running) {

        osEvent e = Thread::signal_wait(buttonSignal, 20);
        if (e.status == osEventSignal) {
            ButtonHandler::ButtonEvent _be = _buttons->getButtonEvent();
            switch (_be) {
                case ButtonHandler::sw1_press:
                    break;
                case ButtonHandler::sw2_press:
                    break;
                case ButtonHandler::sw1_hold:
                    return true;
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
	return false;
}
