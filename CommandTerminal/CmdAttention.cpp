#include "CmdAttention.h"

CmdAttention::CmdAttention(mDot* dot) : Command(dot, "Attention", "AT", "Attention") 
{
    _help = std::string(text()) + ": " + std::string(desc());
}

CmdAttention::CmdAttention(mDot* dot, const char* name, const char* text, const char* desc) : Command(dot, name, text, desc)
{
    
}

uint32_t CmdAttention::action(std::vector<std::string> args) {
    return 0;
}

