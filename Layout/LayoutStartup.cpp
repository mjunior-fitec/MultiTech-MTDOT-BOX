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
    clear();
    startUpdate();

    writeImage(_iLogo._col, _iLogo._row, _iLogo._bmp);
    writeField(_lName._col, _lName._row, _lName._value);
    writeField(_lInfo._col, _lInfo._row, _lInfo._value);
    writeField(_lVersion._col, _lVersion._row, _lVersion._value);
    writeField(_fVersion._col, _fVersion._row, MTDOT_BOX_VERSION, sizeof(MTDOT_BOX_VERSION));

    endUpdate();
}
