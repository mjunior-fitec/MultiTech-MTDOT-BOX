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
// Sensor header
#include "SensorHandler.h"
// mode objects
#include "ModeJoin.h"
#include "ModeSingle.h"
#include "ModeSweep.h"
#include "ModeDemo.h"
#include "ModeConfig.h"
#include "ModeSemtech.h"
// misc heders
#include "FileName.h"
#include <string>

#define DISABLE_DUTY_CYCLE true


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
LoRaHandler* loraHandler;
mDot* dot;

// GPS
GPSPARSER* gps;
MTSSerial gps_serial(XBEE_DOUT, XBEE_DIN, 256, 2048);

// Sensors
SensorHandler* sensors;

// Modes
ModeJoin* modeJoin;
ModeSingle* modeSingle;
ModeSweep* modeSweep;
ModeDemo* modeDemo;
ModeConfig* modeConfig;
ModeSemtech* modeSemtech;

// Serial debug port
Serial debug(USBTX, USBRX);

// Survey Data File
char file_name[] = "SurveyData.txt";

// Prototypes
void mainMenu();

int main() {
    debug.baud(115200);

    lcd = new DOGS102(lcd_spi, lcd_spi_cs, lcd_cd);
    // NCP5623B::LEDs 1 & 2 are the screen backlight - not used on default build
    // NCP5623B::LED3 is EVB LED2
    led_cont = new NCP5623B(led_i2c);

    main_id = Thread::gettid();
    buttons = new ButtonHandler(main_id);
    dot = mDot::getInstance();

    dot->setDisableDutyCycle(DISABLE_DUTY_CYCLE);

    // Seed the RNG
    srand(dot->getRadioRandom());

    loraHandler = new LoRaHandler(main_id);
    gps = new GPSPARSER(&gps_serial, led_cont);
    sensors = new SensorHandler();

    led_cont->setLEDCurrent(16);

    MTSLog::setLogLevel(MTSLog::TRACE_LEVEL);


    modeJoin = new ModeJoin(lcd, buttons, dot, loraHandler, gps, sensors);
    modeSingle = new ModeSingle(lcd, buttons, dot, loraHandler, gps, sensors);
    modeSweep = new ModeSweep(lcd, buttons, dot, loraHandler, gps, sensors);
    modeDemo = new ModeDemo(lcd, buttons, dot, loraHandler, gps, sensors);
    modeConfig = new ModeConfig(lcd, buttons, dot, loraHandler, gps, sensors);
    modeSemtech = new ModeSemtech(lcd, buttons, dot, loraHandler, gps, sensors);

    osDelay(1000);
    logInfo("%sGPS detected", gps->gpsDetected() ? "" : "no ");

    // display startup screen for 3 seconds
    LayoutStartup ls(lcd, dot);
    ls.display();
    ls.updateGPS(gps->gpsDetected());
    osDelay(3000);

    logInfo("displaying main menu");
    mainMenu();

    return 0;
}

void mainMenu() {
    bool mode_selected = false;
    std::string selected;
    std::string product;
    
    typedef enum {
        demo = 1,
        config,
        single,
        sweep,
        semtech
    } menu_items;

    std::string menu_strings[] = {
        "Select Mode",
        "LoRa Demo",
        "Configuration",
        "Survey Single",
        "Survey Sweep",
        "Semtech"
    };

    std::vector<std::string> items;
    items.push_back(menu_strings[demo]);
    items.push_back(menu_strings[config]);
    items.push_back(menu_strings[single]);
    items.push_back(menu_strings[sweep]);
    items.push_back(menu_strings[semtech]);

    while (true) {
        product = "MTDOT-BOX/EVB ";
        product += mDot::FrequencyBandStr(dot->getFrequencyBand()).substr(3);

        // reset session between modes
        dot->resetNetworkSession();
        loraHandler->resetActivityLed();
        LayoutScrollSelect menu(lcd, items, product, menu_strings[0]);
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
        else if (selected == menu_strings[semtech]) {
                if(dot->getFrequencyBand()==mDot::FB_868)modeJoin->start();
                modeSemtech->start();
        }

        mode_selected = false;
    }
}



