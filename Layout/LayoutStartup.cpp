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
    _fName(14, 3, 3),
    _fVersion(8, 5, 9),
    _fId(0, 7, 17),
    _fGps(0, 6, 17),
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
    writeField(_fName, mDot::FrequencyBandStr(_dot->getFrequencyBand()).substr(3));
    writeField(_fVersion, version);
    writeField(_fId, id);

    endUpdate();
}

void LayoutStartup::updateGPS(bool gps) {
    char buf[32];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%sGPS Detected", gps ? "" : "No ");
    writeField(_fGps, buf, size);
}

