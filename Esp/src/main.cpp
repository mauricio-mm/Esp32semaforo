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
bool publish = true;
bool maintenance = false;
bool on_off = false;

volatile bool fallingEdgeDetected = false;

void IRAM_ATTR onFalling() {
    fallingEdgeDetected = true;
}

void setup() 
{  
  config(&semaphoro);

  pinMode(SEMAPHORE_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SEMAPHORE_BUTTON), onFalling, FALLING);

  WIFIConnect(&espClient);

  MQTT.setServer(mqtt_broker, mqtt_port);   
  MQTT.setCallback(callback);

  state     = 0;
  counter = 0;
  in      = 0;
  wait    = false;
  timer   = false;
  publish = true;
  on_off  = false;
  start   = 0;
} 

void loop() 
{
  MQTT.loop();

  if (!maintenance) {
    if (publish) {
        if(!MQTT.connected()) MQTTConnect(&MQTT);
        if(WiFi.status() != WL_CONNECTED) WIFIConnect(&espClient);

        String payload = timerStateToString(state);
        Serial.println("State " + String(state) + " :" + payload);
        publish_data(&MQTT, topic_semaphoro, payload.c_str()); 
        publish = false;   
      }

      if (fallingEdgeDetected && !timer)
      {
        Serial.println("wait press");

        timer = true;
        fallingEdgeDetected = false;

        detachInterrupt(digitalPinToInterrupt(SEMAPHORE_BUTTON));
      }
        

      if (counter < (frequency * 10))
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
            if (start > 2)
            {
              timer = false;
              wait  = false;
              start = 0;
              attachInterrupt(digitalPinToInterrupt(SEMAPHORE_BUTTON),onFalling,FALLING);
            }else start++;
          }
          else state = (state + 1) % SEMAPHORE_STATE; 

        }else state = (state + 1) % SEMAPHORE_STATE; 

        counter = 0;
        publish = true;
      }
  }else
  { 
      if (publish) {
        if(!MQTT.connected()) MQTTConnect(&MQTT);
        if(WiFi.status() != WL_CONNECTED) WIFIConnect(&espClient);
        String payload;

        if (on_off)
          payload = "{ {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0}, {0,1,0} }";
        else payload = "{ {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0} }";

        Serial.println("State maintenance :" + payload);

        publish_data(&MQTT, topic_semaphoro, payload.c_str()); 
        publish = false;   
      }

      if (counter < ((frequency * 10)/2))
      {
        if (on_off){
          semaphoro_off(&semaphoro);        
          semaphoro_on(&semaphoro, in, 1);
        }else {
          semaphoro_off(&semaphoro); 
        }
   
        delay(1);
        in = (in + 1) % SEMAPHORE_IN_COUNT;
        counter++;
      }else{
        on_off = !on_off;
        counter = 0;
        publish = true;
      };
  }
}
