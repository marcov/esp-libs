#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "wifi.h"

bool wifiHasConnected(void)
{
    static bool was_connected = false;

    bool now_connected = WiFi.status() == WL_CONNECTED;

    if (!was_connected && now_connected)
    {
        Serial.print("WiFi now connected, IP: ");
        Serial.println(WiFi.localIP());

        was_connected = now_connected;
        return true;
    }
    else if (was_connected && !now_connected)
    {
        Serial.println("WiFi now disconnected");
        was_connected = WiFi.status() == WL_CONNECTED;
    }

    return false;
}

static inline uint32_t waitForWifiConnected(void)
{
    uint32_t elapsedMs;
    constexpr uint32_t sleepMs = 500;

    elapsedMs = 0;

    while (!wifiHasConnected())
    {
        delay(sleepMs);
        elapsedMs += sleepMs;
        Serial.print(".");
    }

    return elapsedMs;
}

void wifiSetup(bool mode_ap, bool allowSleep, const char *ssid, const char *password)
{
    WiFi.persistent(false);

    WiFi.disconnect();

    Serial.println("WiFi start mode: " +
                   String(mode_ap ? "AP" : "STA") +
                   " ssid: " + ssid +
                   " password: " + password);

    if (mode_ap)
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ssid, password);
    }
    else
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        WiFi.setAutoReconnect(true);

        waitForWifiConnected();

        WiFi.setSleepMode(allowSleep ? WIFI_LIGHT_SLEEP : WIFI_NONE_SLEEP);
    }
}
