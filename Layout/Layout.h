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

#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include "DOGS102.h"
#include <string>

class Label {
    public:
        Label(uint8_t col, uint8_t row, std::string value);

        uint8_t _col;
        uint8_t _row;
        std::string _value;
};

class Field {
    public:
        Field(uint8_t col, uint8_t row, uint8_t maxSize);

        uint8_t _col;
        uint8_t _row;
        uint8_t _maxSize;
};

class Image {
    public:
        Image(uint8_t col, uint8_t row, const uint8_t* bmp);

        uint8_t _col;
        uint8_t _row;
        const uint8_t* _bmp;
};

class Layout {
    public:
        Layout(DOGS102* lcd);
        ~Layout();

        virtual void display() = 0;
        
    protected:
        void clear();
        void startUpdate();
        void endUpdate();
        bool writeLabel(const Label& label);
        bool writeField(const Field& field, const std::string& value, bool apply = false);
        bool writeField(const Field& field, const char* value, size_t size, bool apply = false);
        bool writeImage(const Image& image, bool apply = false);
        void removeField(const Field& field);

    private:
        bool writeText(uint8_t col, uint8_t row, const char* value, size_t size);
        bool writeBmp(uint8_t col, uint8_t row, const uint8_t* bmp);

        DOGS102* _lcd;
};

#endif
