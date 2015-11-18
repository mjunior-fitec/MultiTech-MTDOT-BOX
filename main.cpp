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
#include "LayoutConfig.h"
#include "LayoutDemoHelp.h"
#include "LayoutSingleHelp.h"
#include "LayoutSweepHelp.h"
// button header
#include "ButtonHandler.h"
// LoRa header
#include "LoRaHandler.h"
// mode objects
#include "ModeJoin.h"
// misc heders
#include <string>

// LCD and backlight controllers
SPI lcd_spi(SPI1_MOSI, SPI1_MISO, SPI1_SCK);
I2C backlight_i2c(I2C_SDA, I2C_SCL);
DigitalOut lcd_spi_cs(SPI1_CS, 1);
DigitalOut lcd_cd(XBEE_ON_SLEEP, 1);
DOGS102* lcd;
NCP5623B* lcd_backlight;

// Thread informaiton
osThreadId main_id;

// Button controller
ButtonHandler* buttons;

// LoRa controller
LoRaHandler* lora;
mDot* dot;

// Modes
ModeJoin* modeJoin;

// Serial debug port
Serial debug(USBTX, USBRX);
mts::MTSSerial serial(USBTX, USBRX, 512, 512);

// Prototypes
void mainMenu();
void join();
void configuration();
void loraDemo();
void surveySingle();
void surveySweep();

int main() {
    debug.baud(115200);

    lcd = new DOGS102(lcd_spi, lcd_spi_cs, lcd_cd);
    lcd_backlight = new NCP5623B(backlight_i2c);

    main_id = Thread::gettid();
    buttons = new ButtonHandler(main_id);
    dot = mDot::getInstance();
    lora = new LoRaHandler(main_id);

    modeJoin = new ModeJoin(lcd, buttons, dot, lora, dot->getFrequencyBand());

    // display startup screen for 3 seconds
    LayoutStartup ls(lcd);
    ls.display();
    osDelay(3000);

    // start of temporary stuff!
    if (dot->getFrequencyBand() == mDot::FB_915)
        dot->setFrequencySubBand(mDot::FSB_7);
    dot->setJoinMode(mDot::OTA);
    dot->setNetworkName("mikes_lora_network");
    dot->setNetworkPassphrase("password_123");
    dot->setAck(1);
    // end of temporary stuff!

    //MTSLog::setLogLevel(MTSLog::TRACE_LEVEL);
    MTSLog::setLogLevel(MTSLog::INFO_LEVEL);
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
                loraDemo();
        } else if (selected == menu_strings[config]) {
            configuration();
        } else if (selected == menu_strings[single]) {
            if (modeJoin->start())
                surveySingle();
        } else if (selected == menu_strings[sweep]) {
            if (modeJoin->start())
                surveySweep();
        }

        mode_selected = false;
    }
}

void configuration() {
    LayoutConfig lc(lcd);

    // clear any stale signals
    osSignalClear(main_id, buttonSignal | loraSignal);

    lc.display();
    logInfo("config mode");

    while (true) {
        osEvent e = Thread::signal_wait(buttonSignal);
        if (e.status == osEventSignal) {
            ButtonHandler::ButtonEvent ev = buttons->getButtonEvent();
            switch (ev) {
                case ButtonHandler::sw1_press:
                    break;
                case ButtonHandler::sw2_press:
                    break;
                case ButtonHandler::sw1_hold:
                    return;
                default:
                    break;
            }
        }
    }
}

void loraDemo() {
    LayoutDemoHelp ldh(lcd);

    // clear any stale signals
    osSignalClear(main_id, buttonSignal | loraSignal);

    ldh.display();
    logInfo("demo mode");

    while (true) {
        osEvent e = Thread::signal_wait(buttonSignal);
        if (e.status == osEventSignal) {
            ButtonHandler::ButtonEvent ev = buttons->getButtonEvent();
            switch (ev) {
                case ButtonHandler::sw1_press:
                    logInfo("trigger TX mode");
                    break;
                case ButtonHandler::sw2_press:
                    logInfo("interval TX mode");
                    break;
                case ButtonHandler::sw1_hold:
                    return;
                default:
                    break;
            }
        }
    }
}

void surveySingle() {
    LayoutSingleHelp lsh(lcd);

    // clear any stale signals
    osSignalClear(main_id, buttonSignal | loraSignal);

    lsh.display();
    logInfo("survey single mode");

    while (true) {
        osEvent e = Thread::signal_wait(buttonSignal);
        if (e.status == osEventSignal) {
            ButtonHandler::ButtonEvent ev = buttons->getButtonEvent();
            switch (ev) {
                case ButtonHandler::sw1_press:
                    logInfo("datarate/power");
                    break;
                case ButtonHandler::sw2_press:
                    logInfo("start survey");
                    break;
                case ButtonHandler::sw1_hold:
                    return;
                default:
                    break;
            }
        }
    }
}

void surveySweep() {
    LayoutSweepHelp lsh(lcd);

    // clear any stale signals
    osSignalClear(main_id, buttonSignal | loraSignal);

    lsh.display();
    logInfo("survey sweep mode");

    while (true) {
        osEvent e = Thread::signal_wait(buttonSignal);
        if (e.status == osEventSignal) {
            ButtonHandler::ButtonEvent ev = buttons->getButtonEvent();
            switch (ev) {
                case ButtonHandler::sw1_press:
                    logInfo("cancel");
                    break;
                case ButtonHandler::sw2_press:
                    logInfo("start sweep");
                    break;
                case ButtonHandler::sw1_hold:
                    return;
                default:
                    break;
            }
        }
    }
}

