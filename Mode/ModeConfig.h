#ifndef __MODECONFIG_H__
#define __MODECONFIG_H__

#include "Mode.h"
#include "LayoutConfig.h"  
#include "mbed.h"
#include "MTSSerial.h"
#include "Commands.h"
#include "mDot.h"
#include "ButtonHandler.h"

class ModeConfig : public Mode {

public:

    ModeConfig(DOGS102* lcd, mts::MTSSerial& serial, mDot* dot, ButtonHandler* buttons);
    
    // Command error text...
    static const char command_error[];
    
    // Response texts...
    static const char newline[];
    static const char done[];
    static const char error[];

    bool start();
    
private: 

    static void idle(void const* args) {
        while (1)
            __WFI();
    }

    mts::MTSSerial& _serial;
    static mts::MTSSerial* _serialp;

	LayoutConfig _lc;
	ButtonHandler::ButtonEvent _be;
	ButtonHandler* _buttons;	
    mDot* _dot;
    mDot::Mode _mode;
    std::vector<Command*> _commands;
    Thread _idle_thread;
    bool _serial_up;

    void addCommand(Command* cmd);
    
    void printHelp();

    bool readable();
    bool writeable();
    char read();
    void write(const char* message);
    void writef(const char* format, ... );
    
};

#endif // __MODECONFIG_H__
