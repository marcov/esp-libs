#ifndef __MQTT_H__
#define __MQTT_H__

#include <stdbool.h>

#define MQTT_PORT_NUMBER 1883

/******************************************************************************/

typedef void (*mqtt_cb_t)(unsigned char *payload, unsigned int length);
typedef void (*mqtt_pub_cb_t)(const char **topic, char *msg);

typedef struct {
    const char *topic;
    mqtt_cb_t  cb;
} mqtt_subitem_t;

typedef struct {
    const char *topic;
    const char * (*getmsg)(void);
    unsigned   period_ms;
    unsigned   _lastmsg_ts;
} mqtt_pubitem_t;

typedef struct
{
    const char           *server;
    mqtt_pubitem_t       *publist;
    const mqtt_subitem_t *sublist;
    const char *username;
    const char *password;
} mqtt_cfg_t;

/******************************************************************************/

void    mqtt_setup(const mqtt_cfg_t *p_cfg);

void    mqtt_loop(void);

bool    mqtt_check_reconnect(void);

void    mqtt_async_publish(const char *topic, const char *msg);

#endif /* #ifndef __MQTT_H__ */
