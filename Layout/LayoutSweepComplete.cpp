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

#include "LayoutSweepComplete.h"

LayoutSweepComplete::LayoutSweepComplete(DOGS102* lcd)
  : Layout(lcd),
    _lId(0, 0, "ID"),
    _lInfo(0, 1, "Sweep Finished"),
    _lPass(0, 5, "Pass"),
    _lFail(9, 5, "Fail"),
    _lSw2(0, 7, "Sweep"),
    _fId(2, 0, 5),
    _fPass(5, 5, 2),
    _fFail(14, 5, 2)
{}

LayoutSweepComplete::~LayoutSweepComplete() {}

void LayoutSweepComplete::display() {
    clear();
    startUpdate();

    writeLabel(_lId);
    writeLabel(_lInfo);
    writeLabel(_lPass);
    writeLabel(_lFail);
    writeLabel(_lSw2);

    endUpdate();
}

void LayoutSweepComplete::updateId(uint32_t id) {
    char buf[16];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%lu", id);
    writeField(_fId, buf, size, true);
}

void LayoutSweepComplete::updatePass(uint8_t pass) {
    char buf[8];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%u", pass);
    writeField(_fPass, buf, size, true);
}

void LayoutSweepComplete::updateFail(uint8_t fail) {
    char buf[8];
    size_t size;

    size = snprintf(buf, sizeof(buf), "%u", fail);
    writeField(_fFail, buf, size, true);
}

