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

    while (v.size() < field._maxSize)
        v += " ";

    ret = writeText(field._col, field._row, v.c_str(), field._maxSize);

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

bool Layout::writeText(uint8_t col, uint8_t row, const char* value, size_t size) {
    _lcd->writeText(col*6, row, font_6x8, value, size);

    return true;
}

bool Layout::writeBmp(uint8_t col, uint8_t row, const uint8_t* bmp) {
    _lcd->writeBitmap(col*6, row, bmp);

    return true;
}

