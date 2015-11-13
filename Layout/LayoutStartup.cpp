#include "LayoutStartup.h"
#include "MultiTech_Logo.h"
#include "version.h"

LayoutStartup::LayoutStartup(DOGS102* lcd)
  : Layout(lcd),
    _lName(0, 3, "MTDOT-BOX/EVB"),
    _lInfo(0, 4, "Factory Firmware"),
    _lVersion(0, 5, "Version"),
    _fVersion(8, 5, 9),
    _iLogo(0, 0, MultiTech_Logo)
{}

LayoutStartup::~LayoutStartup() {}

void LayoutStartup::display() {
    std::string version = MTDOT_BOX_VERSION;

    clear();
    startUpdate();

    writeImage(_iLogo);
    writeLabel(_lName);
    writeLabel(_lInfo);
    writeLabel(_lVersion);
    writeField(_fVersion, version);

    endUpdate();
}
