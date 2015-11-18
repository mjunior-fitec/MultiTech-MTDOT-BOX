// mbed headers
#include "mbed.h"
#include "rtos.h"
// MTS headers
#include "mDot.h"
#include "MTSLog.h"
#include "MTSText.h"
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
#include "LayoutJoin.h"
#include "LayoutConfig.h"
// button header
#include "ButtonHandler.h"
// LoRa header
#include "LoRaHandler.h"
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

// Serial debug port
Serial debug(USBTX, USBRX);

// Prototypes
void mainMenu();
void join();
void configuration();

int main() {
    debug.baud(115200);

    lcd = new DOGS102(lcd_spi, lcd_spi_cs, lcd_cd);
    lcd_backlight = new NCP5623B(backlight_i2c);

    main_id = Thread::gettid();
    buttons = new ButtonHandler(main_id);
    dot = mDot::getInstance();

    // display startup screen for 3 seconds
    LayoutStartup ls(lcd);
    ls.display();
    osDelay(3000);

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
            join();
        } else if (selected == menu_strings[config]) {
            configuration();
        } else if (selected == menu_strings[single]) {
            join();
        } else if (selected == menu_strings[sweep]) {
            join();
        }

        mode_selected = false;
    }
}

void join() {
    uint32_t attempts = 1;
    uint32_t next_tx;
    uint8_t rate;
    uint8_t power;
    uint8_t band;
    bool joined = false;
    ButtonHandler::ButtonEvent ev;
    LoRaHandler::LoRaStatus status;

    // clear any stale signals
    osSignalClear(main_id, buttonSignal | loraSignal);

    // start of temporary stuff!
    if (dot->getFrequencyBand() == mDot::FB_915)
        dot->setFrequencySubBand(mDot::FSB_7);
    dot->setJoinMode(mDot::OTA);
    dot->setNetworkName("mikes_lora_network");
    dot->setNetworkPassphrase("password_123");
    dot->setAck(1);
    // end of temporary stuff!

    power = 20;
    band = dot->getFrequencyBand();
    if (band == mDot::FB_915)
        rate = mDot::SF_10;
    else
        rate = mDot::SF_12;

    logInfo("joining");
    LayoutJoin lj(lcd, band);
    lj.display();
    lj.updateStatus("Joining...");

    if (dot->getJoinMode() == mDot::MANUAL) {
        lj.updateId(mts::Text::bin2hexString(dot->getNetworkId()));
        lj.updateKey(mts::Text::bin2hexString(dot->getNetworkKey()));
    } else {
        lj.updateId(dot->getNetworkName());
        lj.updateKey(dot->getNetworkPassphrase());
    }
    if (band == mDot::FB_915)
        lj.updateFsb(dot->getFrequencySubBand());
    // mDot::DataRateStr returns format SF_XX - we only want to display the XX part
    lj.updateRate(dot->DataRateStr(rate).substr(3));
    lj.updatePower(power);

    if (! lora) {
        lora = new LoRaHandler(main_id);
        // give the LoRa worker thread some time to start up
        osDelay(100);
    }
    lora->setDataRate(rate);
    lora->setPower(power);

    while (! joined) {
        next_tx = lora->getNextTx();
        if (next_tx) {
            lj.updateCountdown(next_tx * 1000);
        } else {
            lj.updateAttempt(attempts++);
            lj.updateStatus("Joining...");
            if (! lora->join())
                logError("cannot join - LoRa layer busy");
        }

        osEvent e = Thread::signal_wait(0);
        if (e.status == osEventSignal) {
            if (e.value.signals & buttonSignal) {
                ev = buttons->getButtonEvent();
                switch (ev) {
                    case ButtonHandler::sw1_press:
                        return;
                    case ButtonHandler::sw2_press:
                        break;
                    case ButtonHandler::sw1_hold:
                        return;
                }
            }
            if (e.value.signals & loraSignal) {
                status = lora->getStatus();
                switch (status) {
                    case LoRaHandler::join_success:
                        lj.updateStatus("Join Success!");
                        lj.displayCancel(false);
                        logInfo("joined");
                        joined = true;
                        osDelay(2000);
                        break;

                    case LoRaHandler::join_failure:
                        logInfo("failed to join");
                        break;
                }
            }
        }
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
