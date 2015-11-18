#ifndef __CMDATTENTION_H__
#define __CMDATTENTION_H__

#include "Command.h"

class CmdAttention : public Command {

public:

    CmdAttention(mDot* dot);   
    CmdAttention(mDot* dot, const char* name, const char* text, const char* desc);
    virtual uint32_t action(std::vector<std::string> args);
    
private:
};

#endif // __CMDATTENTION_H__
