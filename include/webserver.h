#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <ESP8266WebServer.h>

typedef struct {
    const char *path;
    void (*server_cb)(void);
} ws_dynamic_page_t;

typedef struct {
    const char *web_path;
    fs::FS     &fsObject;
    const char *fs_path;
} ws_static_page_t;

void webserverInit(bool enable_http_ota, const ws_static_page_t *p_static_pages, const ws_dynamic_page_t *p_dynamic_pages);
void webserverLoop(void);
ESP8266WebServer * webserverGetObjectPtr(void);

#endif /* #ifndef __WEBSERVER_H__ */
