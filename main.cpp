// mbed headers
#include "mbed.h"
#include "rtos.h"
// MTS headers
#include "mDot.h"
#include "MTSLog.h"
// sensor headers
#include "ISL29011.h"
#include "MMA845x.h"
#include "MPL3115A2.h"
#include "GPSPARSER.h"
// display headers
#include "DOGS102.h"
#include "NCP5623B.h"
#include "LayoutStartup.h"
#include "LayoutScrollSelect.h"
// button header
#include "ButtonHandler.h"
// misc heders
#include <string>

// only here for button testing code in main()
#include "font_6x8.h"

// LCD and backlight controllers
SPI lcd_spi(SPI1_MOSI, SPI1_MISO, SPI1_SCK);
I2C backlight_i2c(I2C_SDA, I2C_SCL);
DigitalOut lcd_spi_cs(SPI1_CS, 1);
DigitalOut lcd_cd(XBEE_ON_SLEEP, 1);
DOGS102* lcd;
NCP5623B* lcd_backlight;

// Button controller
ButtonHandler* buttons;

// Serial debug port
Serial debug(USBTX, USBRX);

// Prototypes
void mainMenu();

int main() {
    debug.baud(115200);
    MTSLog::setLogLevel(MTSLog::TRACE_LEVEL);
    logInfo("starting...");

    lcd = new DOGS102(lcd_spi, lcd_spi_cs, lcd_cd);
    lcd_backlight = new NCP5623B(backlight_i2c);

    // display startup screen for 3 seconds
    LayoutStartup ls(lcd);
    ls.display();
    osDelay(3000);

    osThreadId main_id = Thread::gettid();
    buttons = new ButtonHandler(main_id);

    logInfo("displaying main menu");
    mainMenu();

    /* test buttons
    while (true) {
        char buf[16];
        size_t size;

        osEvent e = Thread::signal_wait(buttonSignal);
        if (e.status == osEventSignal) {
            ButtonEvent ev = buttons->getButtonEvent();
            switch (ev) {
                case sw1_press:
                    size = snprintf(buf, sizeof(buf), "SW1 press");
                    break;
                case sw1_hold:
                    size = snprintf(buf, sizeof(buf), "SW1 hold");
                    break;
                case sw2_press:
                    size = snprintf(buf, sizeof(buf), "SW2 press");
                    break;
            }

            lcd->clearBuffer();
            lcd->startUpdate();
            lcd->writeText(0, 0, font_6x8, buf, size);
            lcd->endUpdate();
        }
    }
    */

    return 0;
}

void mainMenu() {
    /*
    std::string menu_strings[] = {
        "MultiTech EVB",
        "Select Mode",
        "LoRa Demo",
        "Configuration",
        "Survey Single",
        "Survey Sweep"
    };
    */

    std::string menu_strings[] = {
        "MultiTech EVB",
        "Select Mode",
        "LoRa Demo",
        "Configuration",
        "Survey Single",
        "Survey Sweep"
    };

    std::vector<std::string> items;
    items.push_back(menu_strings[2]);       // demo
    items.push_back(menu_strings[3]);       // config
    items.push_back(menu_strings[4]);       // single
    items.push_back(menu_strings[5]);       // sweep

    LayoutScrollSelect menu(lcd, items, menu_strings[0], menu_strings[1]);
    menu.display();

    while (true) {
        osEvent e = Thread::signal_wait(buttonSignal);
        if (e.status == osEventSignal) {
            ButtonEvent ev = buttons->getButtonEvent();
            std::string selected;
            switch (ev) {
                case sw1_press:
                    selected = menu.select();
                    logInfo("selected %s", selected.c_str());
                    break;
                case sw2_press:
                    menu.scroll();
                    break;
                case sw1_hold:
                    logInfo("sw1 hold - already in main menu");
                    break;
                default:
                    break;
            }
        }
    }
}
