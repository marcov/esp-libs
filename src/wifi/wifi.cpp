#include <Arduino.h>

#include "wifi.h"

#define ENABLE_DEBUG 0

#if ENABLE_DEBUG
#  define DEBUG(str)  Serial.println(str)
#else
#  define DEBUG(str)  do {/**/} while (0)
#endif

/*!

   @brief WIFI ctor

 */
WIFI::WIFI (
    bool ModeAP,
    bool AllowSleep,
    const char *Ssid,
    const char *Password,
    uint32_t ConnectTimeoutMs
    ) :
    m_ModeAP(ModeAP),
    m_AllowSleep(AllowSleep),
    m_Ssid(Ssid),
    m_Password(Password),
    m_ConnectTimeoutMs(ConnectTimeoutMs)
{
    WiFi.persistent(false);
    m_State = DISCONNECTED;
}

/*!

   @brief Wait until WiFi is connected

   @param TimeoutMs The max timeout in ms to wait. If 0, wait forever.

   @return 0 on success, 1 if timeout expired

 */
int
WIFI::WaitForConnection (
    unsigned long TimeoutMs
    )
{
    uint32_t elapsedMs;
    constexpr uint32_t pollPeriodMs = 500;

    for(elapsedMs = 0;
        !WIFI::IsConnected() && (TimeoutMs == WIFI::WAIT_FOREVER || elapsedMs < TimeoutMs);
        elapsedMs += pollPeriodMs)
    {
        delay(pollPeriodMs);
        elapsedMs += pollPeriodMs;
        Serial.print(".");
    }

    if (elapsedMs < TimeoutMs)
    {
        m_State = CONNECTED;
        return 0;
    }

    return 1;
}

void
WIFI::Connect (
    void
    )
{
    Connect(m_ConnectTimeoutMs);
}

void
WIFI::Connect (
    const unsigned long ConnectTimeoutMs
    )
{
    WiFi.disconnect();
    Serial.println("WiFi start mode: " +
                   String(m_ModeAP ? "AP" : "STA") +
                   " ssid: " + m_Ssid +
                   " password: " + m_Password);

    if (m_ModeAP)
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(m_Ssid, m_Password);
    }
    else
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(m_Ssid, m_Password);
        WiFi.setAutoReconnect(true);

        m_State = CONNECTING;
        WIFI::WaitForConnection(ConnectTimeoutMs);

        if (m_State != CONNECTED)
        {
            DEBUG("DEBU: not connected");
            goto Exit;
        }

        WiFi.setSleepMode(m_AllowSleep ? WIFI_LIGHT_SLEEP : WIFI_NONE_SLEEP);
    }

Exit:
    return;
}

void
WIFI::Loop (
    void ConnectedCallback(void)
    )
{
    switch (m_State)
    {
    case WIFI::DISCONNECTED:
        DEBUG("DEBU: loop disconnected");
    case WIFI::CONNECTED:
        if (!IsConnected())
        {
            Connect();

            if (m_State == WIFI::CONNECTED)
            {
                if (ConnectedCallback != nullptr)
                {
                    ConnectedCallback();
                }
            }
        }
        break;

    case WIFI::CONNECTING:
        DEBUG("DEBU: loop connecting");
        WaitForConnection(m_ConnectTimeoutMs);

        if (m_State == WIFI::CONNECTED)
        {
            if (ConnectedCallback != nullptr)
            {
                DEBUG("DEBU: loop connected!");
                ConnectedCallback();
            }
        }
        break;

    default:
        //
        // SHOULD NEVER HAPPEN!
        //
        Serial.println("ASSERT FAILED");

        for (;;)
            ;;
        break;
    }
}
