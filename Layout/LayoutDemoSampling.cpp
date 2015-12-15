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

    // make sure the string version is used
    writeField(_fInfo, string("No Free Channel"), true);
    size = snprintf(buf, sizeof(buf), "%lu s", seconds);
    writeField(_fSw2, buf, size, true);
}

void LayoutDemoSampling::updateInterval(uint32_t seconds) {
    char buf[32];
    size_t size;

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
    float fx = (float)x;
    fx /= 1024;
    // We can only display 5 characters.
    // For numbers < -1, we display -#.#g. For example -1.3g
    if(fx < -1){
        size = snprintf(buf, sizeof(buf), "%4.1fg", fx);
    }
    // For numbers > -1 and < 0, we display -.##g. For example -.13g
    else if(fx < 0){
        size = snprintf(buf, sizeof(buf), "%4.2fg", fx);
        for(uint8_t i = 1; i < 5; i++ ){
            buf[i] = buf[i+1];
        }
    }
    // For numbers > 0, we display #.##g. For example 0.13g.
    else{
        size = snprintf(buf, sizeof(buf), "%4.2fg", fx);
    }
    writeField(_fAccx, buf, size, true);
}
    
void LayoutDemoSampling::updateAccelerationY(int16_t y) {
    char buf[16];
    size_t size;
    float fy = (float)y;
    fy /= 1024;
    if(fy < -1){
        size = snprintf(buf, sizeof(buf), "%4.1fg", fy);
    }
    else if(fy < 0){
        size = snprintf(buf, sizeof(buf), "%4.2fg", fy);
        for(uint8_t i = 1; i < 5; i++ ){
            buf[i] = buf[i+1];
        }
    }
    else{
        size = snprintf(buf, sizeof(buf), "%4.2fg", fy);
    }
    writeField(_fAccy, buf, size, true);
}
    
void LayoutDemoSampling::updateAccelerationZ(int16_t z) {
    char buf[16];
    size_t size;
    float fz = (float)z;
    fz /= 1024;
    if(fz < -1){
        size = snprintf(buf, sizeof(buf), "%1.1fg", fz);
    }
    else if(fz < 0){
        size = snprintf(buf, sizeof(buf), "%4.2fg", fz);
        for(uint8_t i = 1; i < 5; i++ ){
            buf[i] = buf[i+1];
        }
    }
    else{
        size = snprintf(buf, sizeof(buf), "%1.2fg", fz);
    }
    writeField(_fAccz, buf, size, true);
}

void LayoutDemoSampling::updatePressure(float pressure) {
    char buf[16];
    size_t size;
    size = snprintf(buf, sizeof(buf), "%3.2f KPa", pressure/1000);    
    writeField(_fPres, buf, size, true);
}

void LayoutDemoSampling::updateAltitude(float altitude) {
    char buf[16];
    size_t size;
    size = snprintf(buf, sizeof(buf), "%5.2f m", altitude);    
    writeField(_fAlt, buf, size, true);
}

void LayoutDemoSampling::updateTemperature(float temperature) {
    char buf[16];
    size_t size;
    size = snprintf(buf, sizeof(buf), "%3.2f C", temperature);    
    writeField(_fTemp, buf, size, true);
}

void LayoutDemoSampling::updateLight(float light) {
    char buf[16];
    size_t size;
    size = snprintf(buf, sizeof(buf), "%4.2f lux", light);
    writeField(_fLight, buf, size, true);
}
