/**
  I, Alec Pasion, 000811377 certify that this material is my original work. No other person's work has been used
  without due acknowledgement.
**/
#include <Arduino.h> 
 
// digital input pin definitions 
#define PIN_PIR D5 
#define PIN_BUTTON D6
#define BUTTON_DELAY 500
#define LED_INTERVAL 250
#define BUTTON_STATE_CHECK 0
#define MOTION_CHECK 1
boolean armed = true;
boolean motionDetected = false;
boolean blink_led = false;
boolean alarmActivated = false;
long startTime = 0;
long previousBlink=0;
// ************************************************************* 
void setup() { 
  Serial.begin(115200); 
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(PIN_PIR, INPUT); 
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  Serial.println(" ");
  Serial.println("COMP-10184 - ALARM SYSTEM\nName: Alec Pasion\nStudent ID: 000811377");
} 

void blink(){
  if(motionDetected){
    long currentTime = millis();
    if(currentTime- previousBlink > LED_INTERVAL) {
      previousBlink = currentTime;  
      switch (digitalRead(LED_BUILTIN)) {
        case LOW:
          digitalWrite(LED_BUILTIN,HIGH);
          break;
        case HIGH:
          digitalWrite(LED_BUILTIN,LOW);
          break;
      }
    }
  }
}
void checkButton(){
  int iButton = digitalRead(PIN_BUTTON);
  if (iButton == BUTTON_STATE_CHECK)
  {
    if(!armed){
      motionDetected = false;
      armed = true;
      Serial.println("Motion Alarm Enabled");
    }else{
      if(motionDetected){
        motionDetected = false;
      }
      Serial.println("Motion Alarm Disabled");
      alarmActivated = false;
      armed = false;
    }
    delay(BUTTON_DELAY);
  }
}
void checkMotion(){
  if(armed && motionDetected){
    if(millis()-startTime < 10000){
        blink();
    }else{
      if(!alarmActivated){
        Serial.println("Alarm Activated!");
        alarmActivated = true;
      }
      digitalWrite(LED_BUILTIN,LOW);
    }
  }
  else if(armed && !motionDetected){
    digitalWrite(LED_BUILTIN,HIGH);
    int sensor = digitalRead(PIN_PIR);
    if(sensor == MOTION_CHECK && !motionDetected){
      startTime = millis();
      Serial.println("Motion Detected! Alarm will sound in 10s");
      motionDetected = true;
    }
  }else{
    digitalWrite(LED_BUILTIN,HIGH);
  }
}
// ************************************************************* 
void loop() {
  checkButton();
  checkMotion();
}