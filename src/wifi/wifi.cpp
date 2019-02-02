#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "wifi.h"

static const char* wifi_ssid;
static const char* wifi_password;

static bool was_connected;

bool wifiHasConnected(void)
{
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
        // try reconnect...
        WiFi.begin(wifi_ssid, wifi_password);

        was_connected = WiFi.status() == WL_CONNECTED;
    }

    return false;
}

void wifiSetup(bool mode_ap, bool allowSleep, const char *ssid, const char *password)
{
    was_connected = false;

    WiFi.disconnect();

    Serial.println("WiFi start mode: " +
                   String(mode_ap ? "AP" : "STA") +
                   " ssid: " + ssid +
                   " password: " + password);

    if (mode_ap) {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ssid, password);
    } else {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        WiFi.setAutoReconnect(true);
    }

    WiFi.setSleepMode(allowSleep ? WIFI_LIGHT_SLEEP : WIFI_NONE_SLEEP);
}
