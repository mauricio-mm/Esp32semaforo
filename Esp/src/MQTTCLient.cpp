#include "MQTTClient.h"
#include "main.h"

void MQTTConnect(PubSubClient *MQTT)
{
  while(!MQTT->connected()) 
  {
    if (MQTT->connect(ID_MQTT))
    {
      Serial.println("Conectado ao Broker!");
      MQTT->subscribe(topic_semaphoro);   
      MQTT->subscribe(topic_semaphoro_cb);      
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
  if (strcmp(topic, "lab318/semaphoro_cb") == 0)
  {
      Serial.println("Comando de manutencao");
      maintenance = (payload[0] == '1') ? HIGH : LOW;
  }
}