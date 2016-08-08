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

#ifndef __LAYOUTSEMTECHJOIN_H__
#define __LAYOUTSEMTECHJOIN_H__

#include "Layout.h"
#include "mDot.h"

class LayoutSemtechJoin : public Layout {
    public:
        LayoutSemtechJoin(DOGS102* lcd, uint8_t band);
        ~LayoutSemtechJoin();

        void display();

        void updateId(std::string id);
        void updateKey(std::string key);
        void updateFsb(uint8_t band);
        void updateRate(std::string rate);
        void updatePower(uint32_t power);

    private:
        uint8_t _band;
    
        Label _lId;
        Label _lKey;
        Label _lFsb;
        Label _lRate;
        Label _lPower;
        Label _lJoin;
        
        Field _fId;
        Field _fKey;
        Field _fFsb;
        Field _fRate;
        Field _fPower;
        Field _fStatus;
        Field _fSubBand;
};

#endif


