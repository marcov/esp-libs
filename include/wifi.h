#ifndef __WIFI_H__
#define __WIFI_H__

void wifiSetup(bool mode_ap, bool allowSleep, const char *ssid, const char *password);

bool wifiHasConnected(void);

#endif /* #ifndef __WIFI_H__ */
