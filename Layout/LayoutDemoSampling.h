#ifndef __LAYOUTDEMOSAMPLING_H__
#define __LAYOUTDEMOSAMPLING_H__

#include "Layout.h"
#include "MMA845x.h"

class LayoutDemoSampling : public Layout {
    public:
        LayoutDemoSampling(DOGS102* lcd);
        ~LayoutDemoSampling();

        void display();
        void updateInfo(std::string info);
        void updateSw1(std::string sw1);
        void updateSw2(std::string sw2);
        void updateCountdown(uint32_t seconds);
        void updateInterval(uint32_t seconds);
		void updateAccelerationX(int16_t x);
		void updateAccelerationY(int16_t y);
		void updateAccelerationZ(int16_t z);
		void updatePressure(float pressure);
		void updateAltitude(float altitude);
		void updateTemperature(float temperature);
		void updateLight(float light);

    private:
        Label _lAccx;
        Label _lAccy;
        Label _lAccz;
        Label _lPres;
        Label _lAlt;
        Label _lTemp;
        Label _lLight;
        
        Field _fAccx;
        Field _fAccy;
        Field _fAccz;
        Field _fPres;
        Field _fAlt;
        Field _fTemp;
        Field _fLight;
        Field _fInfo;
        Field _fSw1;
        Field _fSw2;
};

#endif
