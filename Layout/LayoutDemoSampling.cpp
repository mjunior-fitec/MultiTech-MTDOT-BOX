#include "LayoutDemoSampling.h"

LayoutDemoSampling::LayoutDemoSampling(DOGS102* lcd)
  : Layout(lcd),
    _lAccx(0, 0, "AccX"),
    _lAccy(6, 0, "AccY"),
    _lAccz(12, 0, "AccZ"),
    _lPres(0, 2, "Press="),
    _lAlt(0, 3, "Alt="),
    _lTemp(0, 4, "Temp="),
    _lLight(0, 5, "Light="),
    _fAccx(0, 1, 5),
    _fAccy(6, 1, 5),
    _fAccz(12, 1, 5),
    _fPres(6, 2, 11),
    _fAlt(4, 3, 13),
    _fTemp(5, 4, 12),
    _fLight(6, 5, 11),
    _fInfo(0, 6, 17),
    _fSw1(9, 7, 8),
    _fSw2(0, 7, 8)
{}

LayoutDemoSampling::~LayoutDemoSampling() {}

void LayoutDemoSampling::display() {
    clear();
    startUpdate();

    writeLabel(_lAccx);
    writeLabel(_lAccy);
    writeLabel(_lAccz);
    writeLabel(_lPres);
    writeLabel(_lAlt);
    writeLabel(_lTemp);
    writeLabel(_lLight);

    endUpdate();
}

void LayoutDemoSampling::updateInfo(std::string info) {
    writeField(_fInfo, info, true);
}

void LayoutDemoSampling::updateSw1(std::string sw1) {
    writeField(_fSw1, sw1, true);
}

void LayoutDemoSampling::updateSw2(std::string sw2) {
    writeField(_fSw2, sw2, true);
}

void LayoutDemoSampling::updateCountdown(uint32_t seconds) {
    char buf[32];
    size_t size;

    memset(buf, 0, sizeof(buf));
    // for some reason, there's a % character that gets displayed in the last column
    // add the extra spaces to wipe it out
    writeField(_fInfo, "No Free Channel  ", true);
    size = snprintf(buf, sizeof(buf), "%lu s      ", seconds);
    writeField(_fSw2, buf, size, true);
}

void LayoutDemoSampling::updateInterval(uint32_t seconds) {
    char buf[32];
    size_t size;

    memset(buf, ' ', sizeof(buf));
    writeField(_fInfo, buf, sizeof(buf), true);

    memset(buf, 0, sizeof(buf));
    if (seconds < 60)
        size = snprintf(buf, sizeof(buf), "Interval %lu s", seconds);
    else if (seconds < 60 * 60)
        size = snprintf(buf, sizeof(buf), "Interval %lu min", seconds / 60);
    else
        size = snprintf(buf, sizeof(buf), "Interval %lu hr", seconds / (60 * 60));

    writeField(_fInfo, buf, size, true);
}

void LayoutDemoSampling::updateAccelerationX(int16_t x) {
    char buf[16];
    size_t size;
    memset(buf, ' ', sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d", x);
    writeField(_fAccx, buf, size, true);
}
    
void LayoutDemoSampling::updateAccelerationY(int16_t y) {
    char buf[16];
    size_t size;
    memset(buf, ' ', sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d", y);
    writeField(_fAccy, buf, size, true);
}
    
void LayoutDemoSampling::updateAccelerationZ(int16_t z) {
    char buf[16];
    size_t size;
    memset(buf, ' ', sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%d", z);
    writeField(_fAccz, buf, size, true);
}

void LayoutDemoSampling::updatePressure(float pressure) {
    char buf[16];
    size_t size;
    memset(buf, ' ', sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%3.2f KPa", pressure/1000);    
    writeField(_fPres, buf, size, true);
}

void LayoutDemoSampling::updateAltitude(float altitude) {
    char buf[16];
    size_t size;
    memset(buf, ' ', sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%5.2f m", altitude);    
    writeField(_fAlt, buf, size, true);
}

void LayoutDemoSampling::updateTemperature(float temperature) {
    char buf[16];
    size_t size;
    memset(buf, ' ', sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%3.2f C", temperature);    
    writeField(_fTemp, buf, size, true);
}

void LayoutDemoSampling::updateLight(float light) {
    char buf[16];
    size_t size;
    memset(buf, ' ', sizeof(buf));
    size = snprintf(buf, sizeof(buf), "%4.2f lux", light);
    writeField(_fLight, buf, size, true);
}
