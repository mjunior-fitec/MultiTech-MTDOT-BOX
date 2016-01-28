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

#include "LayoutHelp.h"

LayoutHelp::LayoutHelp(DOGS102* lcd)
  : Layout(lcd),
    _lIns1(0, 4, "Hold SW1 any time"),
    _lIns2(0, 5, "for Main Menu"),
    _fMode(0, 0, 17),
    _fDesc(0, 1, 17),
    _fMsg(0, 6, 17),
    _fSw1(9, 7, 8),
    _fSw2(0, 7, 8)
{}

LayoutHelp::~LayoutHelp() {}

void LayoutHelp::display() {
    clear();
    startUpdate();

    writeLabel(_lIns1);
    writeLabel(_lIns2);

    endUpdate();
}

void LayoutHelp::updateMode(std::string mode) {
    writeField(_fMode, mode, true);
}

void LayoutHelp::updateDescription(std::string description) {
    writeField(_fDesc, description, true);
}

void LayoutHelp::updateMsg(std::string msg) {
    writeField(_fMsg, msg, true);
}

void LayoutHelp::updateSw1(std::string s) {
    writeField(_fSw1, s, true);
}

void LayoutHelp::updateSw2(std::string s) {
    writeField(_fSw2, s, true);
}

