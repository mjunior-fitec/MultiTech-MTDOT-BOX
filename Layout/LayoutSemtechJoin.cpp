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

#include "LayoutSemtechJoin.h"

LayoutSemtechJoin::LayoutSemtechJoin(DOGS102* lcd, uint8_t band)
    : Layout(lcd),
      _band(band),
      _lId(0, 1, "NI="),
      _lKey(0, 2, "NK="),
      _lFsb(0, 3, "FSB="),
      _lRate(0, 5, "DR="),
      _lPower(6, 5, "P="),
      _lJoin(0, 7, "Join"),
      _fId(3, 1, 14),
      _fKey(3, 2, 14),
      _fFsb(4, 3, 2),
      _fRate(3, 5, 2),
      _fPower(8, 5, 2),
      _fStatus(0, 0, 17),
      _fSubBand(16,7,1)
{}

LayoutSemtechJoin::~LayoutSemtechJoin() {}

void LayoutSemtechJoin::display()
{
    clear();
    startUpdate();

    writeLabel(_lId);
    writeLabel(_lKey);
    if (_band == mDot::FB_915) {
        writeLabel(_lFsb);
    }
    writeLabel(_lRate);
    writeLabel(_lPower);
    writeLabel(_lJoin);
    
    endUpdate();
}

void LayoutSemtechJoin::updateId(std::string id)
{
    writeField(_fId, id, true);
}

void LayoutSemtechJoin::updateKey(std::string key)
{
    writeField(_fKey, key, true);
}

void LayoutSemtechJoin::updateFsb(uint8_t band)
{
    if (_band == mDot::FB_915) {
    char buf[8];
    size_t size;
    size = snprintf(buf, sizeof(buf), "%u", band);
    writeField(_fFsb, buf, size, true);
    writeField(_fSubBand, buf, size, true);
    }
}

void LayoutSemtechJoin::updateRate(std::string rate)
{
    writeField(_fRate, rate, true);
}

void LayoutSemtechJoin::updatePower(uint32_t power)
{
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", power);
    writeField(_fPower, buf, size, true);
}
