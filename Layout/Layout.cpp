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

bool Layout::writeField(uint8_t col, uint8_t row, std::string field, bool apply) {
    return writeField(col, row, field.c_str(), field.size(), apply);
}

bool Layout::writeField(uint8_t col, uint8_t row, const char* field, size_t size, bool apply) {
    if (apply)
        startUpdate();

    _lcd->writeText(col*6, row, font_6x8, field, size);

    if (apply)
        endUpdate();

    return true;
}

bool Layout::writeImage(uint8_t col, uint8_t row, const uint8_t* bmp, bool apply) {
    if (apply)
        startUpdate();

    _lcd->writeBitmap(col, row, bmp);

    if (apply)
        endUpdate();

    return true;
}

