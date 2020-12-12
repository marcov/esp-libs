#pragma once

#include <ESP8266WiFi.h>

class WIFI
{
public:
    WIFI (
        bool ModeAP,
        bool AllowSleep,
        const char *Ssid,
        const char *Password,
        uint32_t ConnectTimeoutMs
        );

    static constexpr unsigned int WAIT_FOREVER = 0xFFFFFFFF;

    int
    WaitForConnection (
        unsigned long TimeoutMs
        );

    void Connect (
        const unsigned long ConnectTimeoutMs
        );

    void Connect (
        void
        );

    void
    Loop (
        void ConnectedCallback(void)
        );

    bool
    IsConnected (
        )
    {
        return (WiFi.status() == WL_CONNECTED);
    }

private:

    bool m_ModeAP;
    bool m_AllowSleep;
    const char* m_Ssid;
    const char* m_Password;
    uint32_t m_ConnectTimeoutMs;
    enum State
    {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
    } m_State;
};
