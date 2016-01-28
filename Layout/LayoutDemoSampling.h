/* Copyright (c) <2016> <MultiTech Systems>, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
