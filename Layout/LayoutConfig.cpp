#include "LayoutConfig.h"

LayoutConfig::LayoutConfig(DOGS102* lcd)
  : Layout(lcd),
    _lMode(0, 0, "Configuration"),
    _lHelp1(0, 2, "Connect USB debug"),
    _lHelp2(0, 3, "to PC at 115200"),
    _lHelp3(0, 4, "baud to configure")
{}

LayoutConfig::~LayoutConfig() {}

void LayoutConfig::display() {
    clear();
    startUpdate();

    writeLabel(_lMode);
    writeLabel(_lHelp1);
    writeLabel(_lHelp2);
    writeLabel(_lHelp3);

    endUpdate();
}
