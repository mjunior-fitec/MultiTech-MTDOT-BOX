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
