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
// misc heders
#include <string>

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

    LayoutStartup ls(lcd);
    ls.display();

    while (true) {
        logInfo("in loop");
        osDelay(5000);
    }

    return 0;
}
