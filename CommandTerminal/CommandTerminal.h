/**
  ******************************************************************************
  * File Name          : command.h
  * Date               : 18/04/2014 10:57:12
  * Description        : This file provides code for command line prompt
  ******************************************************************************
  *
  * COPYRIGHT(c) 2014 MultiTech Systems, Inc.
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
#include "mbed.h"
#include "MTSSerial.h"
#include "Commands.h"
#include "mDot.h"
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __command_terminal_H__
#define __command_terminal_H__

class CommandTerminal {

public:

    enum WaitType {
        WAIT_JOIN,
        WAIT_RECV,
        WAIT_LINK,
        WAIT_SEND,
        WAIT_NA
    };

    CommandTerminal(mts::MTSSerial& serial, mDot* dot);
    
    // Command prompt text...
    static const char banner[];
    static const char helpline[];
    static const char prompt[];
    
    // Command error text...
    static const char command_error[];
    
    // Response texts...
    static const char help[];
    static const char cmd_error[];
    static const char newline[];
    static const char done[];
    static const char error[];

    // Escape sequence
    static const char escape_sequence[];
    
    static std::string formatPacketData(const std::vector<uint8_t>& data, const uint8_t& format);
    static bool waitForEscape(int timeout, mDot* dot=NULL, WaitType wait=WAIT_NA);

    void start();
    
private: 

    static void idle(void const* args) {
        while (1)
            __WFI();
    }

    mts::MTSSerial& _serial;
    static mts::MTSSerial* _serialp;

    mDot* _dot;
    mDot::Mode _mode;
    std::vector<Command*> _commands;
    Thread _idle_thread;
    bool _sleep_standby;
    DigitalOut _xbee_on_sleep;
    bool _serial_up;

    void addCommand(Command* cmd);
    
    void serial_loop();
    bool autoJoinCheck();

    void printHelp();

    bool readable();
    bool writeable();
    char read();
    void write(const char* message);
    void writef(const char* format, ... );

    void sleep(bool standby);
    void wakeup(bool standby);
    
};

#endif // __command_terminal_H__

