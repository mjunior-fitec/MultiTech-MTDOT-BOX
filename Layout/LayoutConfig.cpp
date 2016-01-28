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

#include "LayoutConfig.h"

LayoutConfig::LayoutConfig(DOGS102* lcd)
  : Layout(lcd),
    _lMode(0, 0, "Configuration"),
    _fHeader(0, 1, 17),
    _fMsg1(0, 3, 17),
    _fMsg2(0, 4, 17),
    _fMsg3(0, 5, 17)
{
    _timer.start();
}

LayoutConfig::~LayoutConfig() {}

void LayoutConfig::display() {
    clear();
    startUpdate();
    writeLabel(_lMode);
    endUpdate();
    update1of3();
}

void LayoutConfig::roll(){
    if(_timer.read_ms() < _duration){
        return;
    }
    if(_screen == 3){
        _screen = 1;
    }
    else
        _screen++;
    
    switch(_screen){
        case 1:
            update1of3();
            break;
        case 2:
            update2of3();
            break;
        case 3:
            update3of3();
            break;
    }
}

void LayoutConfig::update1of3() {
    _screen = 1;
    _timer.reset();
    _duration = 4000;
    writeField(_fHeader, string("1 of 3"), true);
    writeField(_fMsg1, string("Connect to MTMDK"), true);
    writeField(_fMsg2, string("using the 8 pin"), true);
    writeField(_fMsg3, string("ribbon cable."), true);
}

void LayoutConfig::update2of3() {
    _timer.reset();
    _duration = 3000;
    writeField(_fHeader, string("2 of 3"), true);
    writeField(_fMsg1, string("Plug MTMDK into a"), true);
    writeField(_fMsg2, string("PC."), true);
    writeField(_fMsg3, string(""), true);
}

void LayoutConfig::update3of3() {
    _timer.reset();
    _duration = 5000;
    writeField(_fHeader, string("3 of 3"), true);
    writeField(_fMsg1, string("Run com s/w on"), true);
    writeField(_fMsg2, string("new TTY/COM at"), true);
    writeField(_fMsg3, string("115200 bps."), true);
}
