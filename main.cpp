#include "mbed.h"
#include "rtos.h"

#include "DOGS102.h"
#include "ISL29011.h"
#include "MMA845x.h"
#include "MPL3115A2.h"
#include "NCP5623B.h"
#include "GPSPARSER.h"
#include "DisplayManager.h"

#include "mDot.h"
#include "MTSLog.h"

// LCD and backlight controllers
SPI lcd_spi(SPI1_MOSI, SPI1_MISO, SPI1_SCK);
I2C backlight_i2c(I2C_SDA, I2C_SCL);
DigitalOut lcd_spi_cs(SPI1_CS, 1);
DigitalOut lcd_cd(XBEE_ON_SLEEP, 1);
DOGS102* lcd;
NCP5623B* lcd_backlight;
DisplayManager* display;

// Serial debug port
Serial debug(USBTX, USBRX);

int main() {
    debug.baud(115200);
    MTSLog::setLogLevel(MTSLog::TRACE_LEVEL);

    lcd = new DOGS102(lcd_spi, lcd_spi_cs, lcd_cd);
    lcd_backlight = new NCP5623B(backlight_i2c);
    display = new DisplayManager(lcd);

    logInfo("starting...");
    display->displaySplashScreen();

    while (true) {
        logInfo("in loop");
        osDelay(5000);
    }

    return 0;
}
