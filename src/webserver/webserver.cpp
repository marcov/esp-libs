#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <climits>

#include "webserver.h"

#define dputs(...) Serial.println(__VA_ARGS__)

#if !defined(ESP_HTTP_SERVER_PORT)
#define ESP_HTTP_SERVER_PORT 80
#endif

ESP8266WebServer httpServer(ESP_HTTP_SERVER_PORT);
ESP8266HTTPUpdateServer httpUpdater;

static void serveNotFound(void)
{
    String message = "File Not Found\n\n";

    message += "URI: ";
    message += httpServer.uri();
    message += "\nMethod: ";
    message += (httpServer.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += httpServer.args();
    message += "\n";

    for (uint8_t i=0; i < httpServer.args(); i++)
    {
        message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
    }
    httpServer.send(404, "text/plain", message);
}

ESP8266WebServer * webserverGetObjectPtr(void)
{
    return &httpServer;
}

void webserverInit(bool enable_http_ota, const ws_static_page_t *p_static_pages, const ws_dynamic_page_t *p_dynamic_pages)
{
    if (p_dynamic_pages != NULL) {
        for (const ws_dynamic_page_t *p_page = &p_dynamic_pages[0]; p_page->path != NULL; p_page++)
        {
            httpServer.on(p_page->path, p_page->server_cb);
        }
    }

    if (p_static_pages != NULL) {
        for (const ws_static_page_t *p_page = &p_static_pages[0]; p_page->web_path != NULL; p_page++)
        {
            httpServer.serveStatic(p_page->web_path, p_page->fsObject, p_page->fs_path);
        }
    }

    httpServer.onNotFound(serveNotFound);

    httpServer.begin();
    dputs("HTTP httpServer started");

    if (enable_http_ota) {
        httpUpdater.setup(&httpServer);
    }
}


void webserverLoop(void)
{
    httpServer.handleClient();
}
