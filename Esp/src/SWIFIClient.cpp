#include "SWIFIClient.h"

const char* ssid     = "POCOX6Pro";    
const char* password = "esppucrs";

void WIFIConnect(WiFiClient *espClient)
{
  Serial.println("Conectando a rede wifi!");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Conectando a rede wifi....");
  }
  Serial.println("Conectado a rede wifi");
}