#include "MQTTClient.h"

void MQTTConnect(PubSubClient *MQTT)
{
  while(!MQTT->connected()) 
  {
    if (MQTT->connect(ID_MQTT))
    {
      Serial.println("Conectado ao Broker!");
      MQTT->subscribe(topic_semaphoro);      
    } 
    else 
    {
      Serial.print("Falha na conexão. O status é: ");
      Serial.print(MQTT->state());      
    }
  }
}

void publish_data(PubSubClient *MQTT,const char *topic, String data)
{
  MQTT->publish(topic, data.c_str());
}

void callback(char *topic, byte *payload, unsigned int length) 
{
  
}