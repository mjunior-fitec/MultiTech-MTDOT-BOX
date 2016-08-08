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

#ifndef __MODEDATA_H__
#define __MODEDATA_H__

#include "Mode.h"
#include "LayoutData.h"
#include "LayoutHelp.h"
//max size of line plus one
#define ONELINEMAX 93


class ModeData : public Mode
{

public:
    ModeData(DOGS102* lcd, ButtonHandler* buttons, mDot* dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors);
    ~ModeData();

    bool start();


private:



    bool _last;
    string _str;

    int32_t  _pos, _buf_size, _indexUpdate, _prev;
    uint32_t _index;

    char _buf[ONELINEMAX];

    mDot::mdot_file _file;

    LayoutData _data;
    LayoutHelp _help;
    LayoutData::singleLine _line;

    bool checkFile();
    bool readFile();
    string parse();
    void getLine();
    void displayData();
    void forward();
    void back();
    void configSw();


};

#endif