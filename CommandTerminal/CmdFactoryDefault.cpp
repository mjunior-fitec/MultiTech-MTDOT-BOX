#include "CmdFactoryDefault.h"

CmdFactoryDefault::CmdFactoryDefault(mDot* dot) : Command(dot, "Reset Factory Defaults", "AT&F", "Reset current configuration to factory defaults")
{
    _help = std::string(text()) + ": " + std::string(desc());
}


uint32_t CmdFactoryDefault::action(std::vector<std::string> args)
{
    _dot->resetConfig();

//Factory defaults for the DotBox.
    _dot->setTxDataRate(mDot::SF_7);
    _dot->setFrequencySubBand(1);
    _dot->setWakeDelay(242);		//DotBox +MaxSize is stored here. Default is 242.
    _dot->setWakeInterval(11);		//DotBox +MinSize is stored here. Default is 11.
    _dot->setWakeMode(20);		//DotBox +MaxPwr is stored here. Default is 20.
    _dot->setWakeTimeout(2);		//DotBox +MinPwr is stored here. Default is 2.
    _dot->setStartUpMode(0);		//DotBox +Data is stored here. Default is 0.
    _dot->setVerbose(1);
    _dot->setEcho(1);
    _dot->setAck(1);
    _dot->setJoinMode(mDot::OTA);
    _dot->setNetworkName("MultiTech");
    _dot->setNetworkPassphrase("MultiTech");
	
    _dot->resetNetworkSession();
    return 0;
}

