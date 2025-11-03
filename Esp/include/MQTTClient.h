#pragma once

#define MQTTClient_h
#ifdef MQTTClient_h

#include <Arduino.h>
#include <PubSubClient.h>

#define ID_MQTT "esp_iot"
#define topic_semaphoro "lab318/semaphoro"

void MQTTConnect(PubSubClient *MQTT);
void publish_data(PubSubClient *MQTT, const char *topic, String data);
void callback(char *topic, byte *payload, unsigned int length);
String timerStateToString(int state);

#endif