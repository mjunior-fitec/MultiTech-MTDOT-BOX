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

#ifndef __MODECONFIG_H__
#define __MODECONFIG_H__

#include "Mode.h"
#include "LayoutConfig.h"  
#include "mbed.h"
#include "MTSSerial.h"
#include "Commands.h"

class ModeConfig : public Mode {

public:

    ModeConfig(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors);
    
    // Command error text...
    static const char command_error[];
    
    // Response texts...
    static const char newline[];
    static const char done[];
    static const char error[];

    bool start();
    
private: 

    mts::MTSSerial _serial;
    LayoutConfig _lc;
    std::vector<Command*> _commands;

    void addCommand(Command* cmd);
    
    void printHelp();

    bool readable();
    bool writeable();
    char read();
    void write(const char* message);
    void writef(const char* format, ... );
    
};

#endif // __MODECONFIG_H__
