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
#include "ModeConfig.h"
// misc heders
#include <string>
/*------------------------------------------------------*/
#include "SensorHandler.h"
/*------------------------------------------------------*/


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
ModeSingle* modeSingle;
ModeSweep* modeSweep;
ModeConfig* modeConfig;

// Serial debug port
Serial debug(USBTX, USBRX);

// Prototypes
void mainMenu();
void join();
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

    modeJoin = new ModeJoin(lcd, buttons, dot, lora);
    modeSingle = new ModeSingle(lcd, buttons, dot, lora);
    modeSweep = new ModeSweep(lcd, buttons, dot, lora);
    modeConfig = new ModeConfig(lcd, buttons, dot, lora);

    // display startup screen for 3 seconds
    LayoutStartup ls(lcd, dot);
    ls.display();
    osDelay(3000);

    MTSLog::setLogLevel(MTSLog::TRACE_LEVEL);
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

/*------------------------------------------------------*/
    SensorHandler *sensors = new SensorHandler();
    MMA845x_DATA accel;
    uint16_t light;
    int32_t num_whole;
    int16_t num_frac;
    uint32_t pressure;
    char txtstr[17];
    while(1){
        accel = sensors->getAcceleration();
        sprintf(txtstr, "Accel: x = %d", accel._x);
        printf("%s\r\n", txtstr);
        sprintf(txtstr, "y = %d", accel._y);
        printf("%s\r\n", txtstr);
        sprintf(txtstr, "z = %d", accel._z );
        printf("%s\r\n", txtstr);
        osDelay(2000);
        
        light = sensors->getLight();
        num_whole = light * 24 / 100;		// 16000 lux full scale .24 lux per bit
        num_frac = light * 24 % 100;
        sprintf(txtstr, "Light=%ld.%02d lux", num_whole, num_frac );
        printf("%s\r\n", txtstr);

        pressure = ((sensors->getBarometer())._baro) >> 12; // convert 32 bit signed to 20 bit unsigned value
        num_whole = pressure >> 2;			// 18 bit integer significant
        num_frac = (pressure & 0x3) * 25;		// 2 bit fractional  0.25 per bit
        sprintf(txtstr,"Press=%ld.%02d Pa", num_whole, num_frac);
        printf("%s\r\n", txtstr);

        num_whole = ((sensors->getBarometer())._temp) / 16;		// 8 bit signed significant integer
        num_frac = (((sensors->getBarometer())._temp) & 0x0F) * 625 / 100;		// 4 bit fractional .0625 per bit
        sprintf(txtstr,"Temp=%ld.%03d C", num_whole, num_frac);
        printf("%s\r\n", txtstr);
    }
    
/*------------------------------------------------------*/


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
                loraDemo();
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

void loraDemo() {
    LayoutHelp lh(lcd);
    lh.display();
    lh.updateMode("LoRa Demo");
    lh.updateDescription("Select TX Method");
    lh.updateSw1(" Trigger");
    lh.updateSw2("Interval");

    // clear any stale signals
    osSignalClear(main_id, buttonSignal | loraSignal);

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

