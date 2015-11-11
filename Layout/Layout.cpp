#include "DisplayManager.h"
#include "font_6x8.h"
#include "MultiTech_Logo.h"
#include "MTSLog.h"
#include "version.h"

// product and version information
std::string DisplayManager::_product_name = "MTDOT-BOX/EVB";
std::string DisplayManager::_program_name = "Factory Firmware";
std::string DisplayManager::_program_version = std::string("Version ") + MTDOT_BOX_VERSION;

DisplayManager::DisplayManager(DOGS102* lcd) : _lcd(lcd) {}

DisplayManager::DisplayManager(DOGS102* lcd, const Layout layout) : _lcd(lcd), _layout(layout) {}

DisplayManager::~DisplayManager() {}

void DisplayManager::displaySplashScreen() {
    _lcd->startUpdate();
    _lcd->writeBitmap(0, 0, MultiTech_Logo);
    _lcd->writeText(0, 3, font_6x8, _product_name.c_str(), _product_name.size());
    _lcd->writeText(0, 4, font_6x8, _program_name.c_str(), _program_name.size());
    _lcd->writeText(0, 5, font_6x8, _program_version.c_str(), _program_version.size());
    _lcd->endUpdate();
}

bool DisplayManager::addLayout(Layout layout) {
    _lcd->clearBuffer();

    _layout = layout;

    _lcd->startUpdate();
    Labels l = _layout.first;
    for (Labels::iterator it = l.begin(); it != l.end(); it++)
        _lcd->writeText(it->column, it->page, font_6x8, it->value, it->size);
    _lcd->endUpdate();

    return true;
}

bool DisplayManager::updateField(const int32_t& id, const char* field, const uint32_t& fieldSize) {
    Fields f = _layout.second;
    for (Fields::iterator it = f.begin(); it!= f.end(); it++) {
        if (it->id == id) {
            _lcd->startUpdate();
            uint8_t size = (fieldSize > it->maxSize) ? it->maxSize : fieldSize;
            _lcd->writeText(it->column, it->page, font_6x8, field, size);
            _lcd->endUpdate();
            return true;
        }
    }

    logError("field ID not found");
    return false;
}

bool DisplayManager::updateField(const int32_t& id, const std::string& field) {
    return updateField(id, field.c_str(), field.size());
}
