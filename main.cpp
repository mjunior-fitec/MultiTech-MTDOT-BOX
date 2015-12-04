// mbed headers
#include "mbed.h"
#include "rtos.h"
// MTS headers
#include "mDot.h"
#include "MTSLog.h"
// display headers
#include "DOGS102.h"
#include "NCP5623B.h"
#include "LayoutStartup.h"
#include "LayoutScrollSelect.h"
#include "LayoutConfig.h"
#include "LayoutHelp.h"
// button header
#include "ButtonHandler.h"
// LoRa header
#include "LoRaHandler.h"
// mode objects
#include "ModeJoin.h"
#include "ModeSingle.h"
#include "ModeSweep.h"
#include "ModeDemo.h"
#include "ModeConfig.h"
// misc heders
#include "FileName.h"
#include <string>

// LCD and LED controllers
SPI lcd_spi(SPI1_MOSI, SPI1_MISO, SPI1_SCK);
I2C led_i2c(I2C_SDA, I2C_SCL);
DigitalOut lcd_spi_cs(SPI1_CS, 1);
DigitalOut lcd_cd(XBEE_ON_SLEEP, 1);
DOGS102* lcd;
NCP5623B* led_cont;

// Thread informaiton
osThreadId main_id;

// Button controller
ButtonHandler* buttons;

// LoRa controller
LoRaHandler* lora;
mDot* dot;

// GPS
GPSPARSER* gps;
MTSSerial gps_serial(XBEE_DOUT, XBEE_DIN, 256, 2048);

// Modes
ModeJoin* modeJoin;
ModeSingle* modeSingle;
ModeSweep* modeSweep;
ModeDemo* modeDemo;
ModeConfig* modeConfig;

// Serial debug port
Serial debug(USBTX, USBRX);

// Survey Data File
char* file_name;

// Prototypes
void mainMenu();
void join();
void loraDemo();
void surveySingle();
void surveySweep();

int main() {
    debug.baud(115200);
    file_name = "SurveyData.txt";

    lcd = new DOGS102(lcd_spi, lcd_spi_cs, lcd_cd);
    led_cont = new NCP5623B(led_i2c);

    main_id = Thread::gettid();
    buttons = new ButtonHandler(main_id);
    dot = mDot::getInstance();
    lora = new LoRaHandler(main_id);
    gps = new GPSPARSER(&gps_serial);

    // NCP5623B::LEDs 1 & 2 are the screen backlight - not used on default build
    // NCP5623B::LED3 is EVB LED2
    led_cont->setPWM(NCP5623B::LED_3, 16);
    led_cont->setLEDCurrent(16);

    MTSLog::setLogLevel(MTSLog::TRACE_LEVEL);

    modeJoin = new ModeJoin(lcd, buttons, dot, lora, gps);
    modeSingle = new ModeSingle(lcd, buttons, dot, lora, gps);
    modeSweep = new ModeSweep(lcd, buttons, dot, lora, gps);
    modeDemo = new ModeDemo(lcd, buttons, dot, lora, gps);
    modeConfig = new ModeConfig(lcd, buttons, dot, lora, gps);

    logInfo("GPS %sdetected", gps->gpsDetected() ? "" : "not ");

    // display startup screen for 3 seconds
    LayoutStartup ls(lcd, dot);
    ls.display();
    osDelay(3000);

    logInfo("displaying main menu");
    mainMenu();

    return 0;
}

void mainMenu() {
    bool mode_selected = false;
    std::string selected;

    typedef enum {
        demo = 2,
        config,
        single,
        sweep
    } menu_items;

    std::string menu_strings[] = {
        "MultiTech EVB",
        "Select Mode",
        "LoRa Demo",
        "Configuration",
        "Survey Single",
        "Survey Sweep"
    };

    std::vector<std::string> items;
    items.push_back(menu_strings[demo]);
    items.push_back(menu_strings[config]);
    items.push_back(menu_strings[single]);
    items.push_back(menu_strings[sweep]);

    while (true) {
        // reset session between modes
        dot->resetNetworkSession();
        LayoutScrollSelect menu(lcd, items, menu_strings[0], menu_strings[1]);
        menu.display();

        while (! mode_selected) {
            osEvent e = Thread::signal_wait(buttonSignal);
            if (e.status == osEventSignal) {
                ButtonHandler::ButtonEvent ev = buttons->getButtonEvent();
                switch (ev) {
                    case ButtonHandler::sw1_press:
                        selected = menu.select();
                        mode_selected = true;
                        break;
                    case ButtonHandler::sw2_press:
                        menu.scroll();
                        break;
                    case ButtonHandler::sw1_hold:
                        break;
                    default:
                        break;
                }
            }
        }

        if (selected == menu_strings[demo]) {
            if (modeJoin->start())
                modeDemo->start();
        } else if (selected == menu_strings[config]) {
            modeConfig->start();
        } else if (selected == menu_strings[single]) {
            if (modeJoin->start())
                modeSingle->start();
        } else if (selected == menu_strings[sweep]) {
            if (modeJoin->start())
                modeSweep->start();
        }

        mode_selected = false;
    }
}

