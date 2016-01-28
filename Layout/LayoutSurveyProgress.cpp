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

#include "LayoutSurveyProgress.h"

LayoutSurveyProgress::LayoutSurveyProgress(DOGS102* lcd)
  : Layout(lcd),
    _lMsg1(5, 2, "Survey"),
    _lMsg2(7, 3, "in"),
    _lMsg3(4, 4, "Progress"),
    _fCountdownLabel(0, 6, 17),
    _fCountdown(0, 7, 9)
{}

LayoutSurveyProgress::~LayoutSurveyProgress() {}

void LayoutSurveyProgress::display() {
    clear();
    startUpdate();

    writeLabel(_lMsg1);
    writeLabel(_lMsg2);
    writeLabel(_lMsg3);

    endUpdate();
}

void LayoutSurveyProgress::updateCountdown(uint32_t seconds) {
    char buf[16];
    size_t size;

    // make sure the string version is used
    writeField(_fCountdownLabel, string("No Free Channel"), true);
    size = snprintf(buf, sizeof(buf), "%lu s", seconds);
    writeField(_fCountdown, buf, size, true);
}

