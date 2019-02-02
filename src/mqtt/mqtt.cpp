#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

extern "C" {
  #include "mqtt.h"
}


static struct
{
    mqtt_pubitem_t       *publist;
    const mqtt_subitem_t *sublist;
    const char *username;
    const char *password;
} mqtt_ctxt;


WiFiClient espClient;
PubSubClient mqttClient((Client &)espClient);


void my_generic_subscribe_handler(char *topic, unsigned char *payload, unsigned int length)
{
    if (topic == NULL)
    {
        return;
    }

    if (mqtt_ctxt.sublist != NULL)
    {
        for (const mqtt_subitem_t *p_item = mqtt_ctxt.sublist; p_item->topic != NULL;p_item++)
        {
            if (strncmp(p_item->topic, topic, strlen(topic)) == 0)
            {
                //found correct handler
                if (p_item->cb != NULL)
                {
                    p_item->cb(payload, length);
                    return;
                }
            }
        }
    }

    Serial.print("Unhandled sub topic=" + String(topic));
}


void mqtt_setup(const mqtt_cfg_t *p_cfg)
{
    mqttClient.setServer(p_cfg->server, MQTT_PORT_NUMBER);
    mqttClient.setCallback(my_generic_subscribe_handler);
    mqtt_ctxt.publist = p_cfg->publist;
    mqtt_ctxt.sublist = p_cfg->sublist;
    mqtt_ctxt.username = p_cfg->username;
    mqtt_ctxt.password = p_cfg->password;
}


void mqtt_async_publish(const char *topic, const char *msg)
{
    mqttClient.publish(topic, msg);
}

void mqtt_loop(void)
{
    long now = millis();

    mqttClient.loop();

    // Check for publishing here
    if (mqtt_ctxt.publist != NULL)
    {
        for (mqtt_pubitem_t *p_item = mqtt_ctxt.publist; p_item->topic != NULL; p_item++)
        {
            const char *msg;

            if (p_item->period_ms == 0 || p_item->getmsg == NULL) continue;

            if (now - p_item->_lastmsg_ts < p_item->period_ms)    continue;

            p_item->_lastmsg_ts = now;
            msg = p_item->getmsg();
            mqttClient.publish(p_item->topic, msg);
        }
    }
}

bool mqtt_check_reconnect(void)
{
    if (mqttClient.connected())         return true;
    if (WiFi.status() != WL_CONNECTED)  return false;

    Serial.print("Attempting MQTT connection...");
    // Create a random mqttClient ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    Serial.print(" --> mqttClient id: ");
    Serial.print(clientId);
    Serial.println();
    int success;

    // Attempt to connect
    if (mqtt_ctxt.username != NULL && mqtt_ctxt.password != NULL) {
       success =  mqttClient.connect(clientId.c_str(), mqtt_ctxt.username, mqtt_ctxt.password);
    } else {
       success =  mqttClient.connect(clientId.c_str());
    }

    if (success) {
        Serial.println("connected");
        // Once connected, publish an announcement...
        mqttClient.publish("powerup", "1");

        // ... and resubscribe
        if (mqtt_ctxt.sublist != NULL)
        {
            for (const mqtt_subitem_t *p_item = mqtt_ctxt.sublist; p_item->topic != NULL; p_item++)
            {
                mqttClient.subscribe(p_item->topic);
            }
        }
        return true;
    }
    else
    {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 5 seconds");
    }

    return false;
}

