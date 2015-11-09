#ifndef __DISPLAYMANAGER_H__
#define __DISPLAYMANAGER_H__

#include "DOGS102.h"

#include <string>
#include <vector>

typedef struct field {
    int32_t id;
    uint8_t column;
    uint8_t page;
    uint8_t maxSize;
} Field;

typedef struct label {
    uint8_t column;
    uint8_t page;
    const char* value;
    uint8_t size;
} Label;

typedef std::vector<Field> Fields;
typedef std::vector<Label> Labels;
typedef std::pair<Labels, Fields> Layout;

class DisplayManager {
    public:
        DisplayManager(DOGS102* lcd);
        DisplayManager(DOGS102* lcd, const Layout layout);
        ~DisplayManager();

        void displaySplashScreen();

        bool addLayout(const Layout layout);

        bool updateField(const int32_t& id, const char* field, const uint32_t& fieldSize);
        bool updateField(const int32_t& id, const std::string& field);

    private:
        Layout _layout;
        DOGS102* _lcd;
        static std::string _product_name;
        static std::string _program_name;
        static std::string _program_version;
};

#endif
