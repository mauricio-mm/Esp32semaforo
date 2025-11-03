#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "semaphoro.h"
#include "SWIFIClient.h"
#include "MQTTClient.h"
#include "utils.h"

SemaphoroPins semaphoro;
WiFiClient    espClient;
PubSubClient MQTT(espClient);

const char* mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;

int32_t frequency = getCpuFrequencyMhz();
int counter;
int state;
int in;
int start;
bool timer;
bool wait;


void setup() 
{  
  config(&semaphoro);
  WIFIConnect(&espClient);

  MQTT.setServer(mqtt_broker, mqtt_port);   
  MQTT.setCallback(callback);

  state   = 0;
  counter = 0;
  in      = 0;
  wait    = false;
  timer   = false;
  start   = 0;
} 

void loop() 
{
  if(!timer)  
   wait = digitalRead(SEMAPHORE_BUTTON);
  
  if(wait == LOW && timer != false)
  {
    timer = true;    
    Serial.println('timer: ' + timer);
  }  
    
  if (counter < (frequency * 100))
  {
    int index = verify_state(timer_state[state][in]);

    semaphoro_off(&semaphoro);        
    semaphoro_on(&semaphoro, in, index);
    delay(1);

    in = (in + 1) % SEMAPHORE_IN_COUNT;
    counter++;
  }else
  {    
    if(timer)
    {
      if((state % 4) == 0)
      {
        Serial.println('state no if: ' + state);
        if (start > 3)
        {
          timer = false;
          wait  = false;
          start = 0;
        }else start++;
      }
      else state = (state + 1) % SEMAPHORE_STATE; 

    }else state = (state + 1) % SEMAPHORE_STATE; 

    Serial.println('state: ' + state);

    if(!MQTT.connected()) MQTTConnect(&MQTT);
    if(WiFi.status() != WL_CONNECTED) WIFIConnect(&espClient);

    String payload = timerStateToString(state);
    publish_data(&MQTT, topic_semaphoro, payload.c_str());    

    state = (state + 1) % SEMAPHORE_STATE; 
    counter = 0;
  }
}
