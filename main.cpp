#include "mbed.h"
#include "rtos.h"

#include "font_6x8.h"
#include "MultiTech_Logo.h"

#include "DOGS102.h"
#include "ISL29011.h"
#include "MMA845x.h"
#include "MPL3115A2.h"
#include "NCP5623B.h"
#include "GPSPARSER.h"

#include "mDot.h"
#include "MTSLog.h"
#include "version.h"

// product and version information
static const std::string product_name = "MTDOT-BOX/EVB";
static const std::string program_name = "Factory Firmware";
static const std::string program_version = MTDOT_BOX_VERSION;

// LCD and backlight controllers
SPI lcd_spi(SPI1_MOSI, SPI1_MISO, SPI1_SCK);
I2C backlight_i2c(I2C_SDA, I2C_SCL);
DigitalOut lcd_spi_cs(SPI1_CS, 1);
DigitalOut lcd_cd(XBEE_ON_SLEEP, 1);
DOGS102* lcd;
NCP5623B* lcd_backlight;

// Serial debug port
Serial debug(USBTX, USBRX);

int main() {
    debug.baud(115200);
    MTSLog::setLogLevel(MTSLog::TRACE_LEVEL);

    lcd = new DOGS102(lcd_spi, lcd_spi_cs, lcd_cd);
    lcd_backlight = new NCP5623B(backlight_i2c);

    logInfo("starting...");

    lcd->startUpdate();
    lcd->writeBitmap(0, 0, MultiTech_Logo);
    lcd->writeText(0, 3, font_6x8, product_name.c_str(), product_name.size());
    lcd->writeText(0, 4, font_6x8, program_name.c_str(), program_name.size());
    lcd->writeText(0, 5, font_6x8, program_version.c_str(), program_version.size());
    lcd->endUpdate();

    logInfo("on screen");

    while (true) {
        logInfo("in loop");
        osDelay(5000);
    }

    return 0;
}
