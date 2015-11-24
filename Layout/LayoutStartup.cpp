#include "LayoutStartup.h"
#include "MultiTech_Logo.h"
#include "version.h"
#include "MTSText.h"

LayoutStartup::LayoutStartup(DOGS102* lcd, mDot* dot)
  : Layout(lcd),
    _dot(dot),
    _lName(0, 3, "MTDOT-BOX/EVB"),
    _lInfo(0, 4, "Factory Firmware"),
    _lVersion(0, 5, "Version"),
    _fVersion(8, 5, 9),
    _fId(0, 7, 17),
    _iLogo(0, 0, MultiTech_Logo)
{}

LayoutStartup::~LayoutStartup() {}

void LayoutStartup::display() {
    std::string version = MTDOT_BOX_VERSION;
    std::string id = mts::Text::bin2hexString(_dot->getDeviceId());

    clear();
    startUpdate();

    writeImage(_iLogo);
    writeLabel(_lName);
    writeLabel(_lInfo);
    writeLabel(_lVersion);
    writeField(_fVersion, version);
    writeField(_fId, id);

    endUpdate();
}
