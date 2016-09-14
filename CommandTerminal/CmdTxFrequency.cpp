#include "CmdTxFrequency.h"
#include <climits>


CmdTxFrequency::CmdTxFrequency(mDot* dot) :
    Command(dot, "Tx Frequency", "AT+TXF", "Set Tx frequency") {
}

uint32_t CmdTxFrequency::action(std::vector<std::string> args)
{
    if (args.size() == 2)
    {
        int frequency = 0;

        sscanf(args[1].c_str(), "%d", &frequency);
        _dot->setTxFrequency(frequency);
    }

    return 0;
}

bool CmdTxFrequency::verify(std::vector<std::string> args)
{
    return true;
}

