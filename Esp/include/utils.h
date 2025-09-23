#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

#include <WiFi.h>
#include <PubSubClient.h>

#include "FS.h"
#include "SPIFFS.h"

#include "main.h"

#define LEDPIN 2

#define ID_MQTT "esp_iot"
#define topic_sinal "lab318/sinal"
#define topic_client "lab318/client"

void WIFIConnect(WiFiClient *espClient);
void MQTTConnect(PubSubClient *MQTT);
void publish_data(PubSubClient *MQTT, const char *topic, String data);
void callback(char *topic, byte *payload, unsigned int length);

#endif