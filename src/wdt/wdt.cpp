#include <Arduino.h>
#include <Ticker.h>
#include "wdt.h"

static unsigned wdt_timeout_ms = 30000;
static unsigned wdtLastKick;
Ticker wdtTimer;


static void ICACHE_RAM_ATTR wdtTimeoutCB(void)
{
    unsigned long timeSinceLastKick = millis() - wdtLastKick;

    if(timeSinceLastKick >= wdt_timeout_ms)
    {
        // save the hit here to eeprom or to rtc memory if needed
        ESP.restart();  // normal reboot
        //ESP.reset();  // hard reset
    }
}

void wdtEnable(void)
{
    wdtKick();
    wdtTimer.attach_ms(wdt_timeout_ms / 3, wdtTimeoutCB);
}


void wdtDisable(void)
{
    wdtTimer.detach();
}

void wdtKick(void)
{
    wdtLastKick = millis();
}

void wdtInit(unsigned timeout_ms)
{
    wdt_timeout_ms = timeout_ms;
}
