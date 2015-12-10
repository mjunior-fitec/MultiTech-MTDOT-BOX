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
