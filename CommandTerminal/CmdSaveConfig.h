
#ifndef __CmdSaveConfig_H__
#define __CmdSaveConfig_H__

#include "Command.h"

class CmdSaveConfig : public Command {

public:

    CmdSaveConfig(mDot* dot);
    virtual uint32_t action(std::vector<std::string> args);

private:
};

#endif // __CmdSaveConfig_H__
