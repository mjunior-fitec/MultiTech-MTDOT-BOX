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

#include "Layout.h"
#include "font_6x8.h"

Label::Label(uint8_t col, uint8_t row, std::string value)
  : _col(col),
    _row(row),
    _value(value)
{}

Field::Field(uint8_t col, uint8_t row, uint8_t maxSize)
  : _col(col),
    _row(row),
    _maxSize(maxSize)
{}

Image::Image(uint8_t col, uint8_t row, const uint8_t* bmp)
  : _col(col),
    _row(row),
    _bmp(bmp)
{}

Layout::Layout(DOGS102* lcd) : _lcd(lcd) {}

Layout::~Layout() {}

void Layout::clear() {
    _lcd->clearBuffer();
}

void Layout::startUpdate() {
    _lcd->startUpdate();
}

void Layout::endUpdate() {
    _lcd->endUpdate();
}

bool Layout::writeLabel(const Label& label) {
    return writeText(label._col, label._row, label._value.c_str(), label._value.size());
}

bool Layout::writeField(const Field& field, const std::string& value, bool apply) {
    bool ret;
    std::string v = value;

    if (apply)
        startUpdate();

    // fill the whole length with blank space in case the previous value was longer than this one
    while (v.size() < field._maxSize)
        v += " ";

    ret = writeText(field._col, field._row, v.c_str(), field._maxSize);

    if (apply)
        endUpdate();

    return true;
}

bool Layout::writeField(const Field& field, const char* value, size_t size, bool apply) {
    bool ret;
    char buf[32];

    // fill the whole length with blank space in case the previous value was longer than this one
    memset(buf, ' ', sizeof(buf));

    if (apply)
        startUpdate();

    snprintf(buf, sizeof(buf), "%s", value);
    // wipe out the null character - the LCD driver will just skip that character otherwise
    buf[size] = ' ';

    ret = writeText(field._col, field._row, buf, field._maxSize);

    if (apply)
        endUpdate();

    return true;
}

bool Layout::writeImage(const Image& image, bool apply) {
    bool ret;

    if (apply)
        startUpdate();

    ret = writeBmp(image._row, image._col, image._bmp);

    if (apply)
        endUpdate();

    return ret;
}

void Layout::removeField(const Field& field) {
    startUpdate();
    std::string s(' ', field._maxSize);
    writeText(field._row, field._col, s.c_str(), s.size());
    endUpdate();
}

bool Layout::writeText(uint8_t col, uint8_t row, const char* value, size_t size) {
    _lcd->writeText(col*6, row, font_6x8, value, size);

    return true;
}

bool Layout::writeBmp(uint8_t col, uint8_t row, const uint8_t* bmp) {
    _lcd->writeBitmap(col*6, row, bmp);

    return true;
}

