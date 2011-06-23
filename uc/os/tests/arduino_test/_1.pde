#include <SPI.h>

const int button_pin = 2;
const int cs_pin = 10;

#include "mcp.h"

byte last_state;

void setup() {
  pinMode(button_pin, INPUT);
  last_state = digitalRead(button_pin);

  Serial.begin(9600);
  pinMode(cs_pin, OUTPUT);
  
  SPI.setClockDivider(16);
  SPI.begin(); 
}

void worker() {
  mcp_probe();
}

void loop() {
//  return;
//  byte state = digitalRead(button_pin);

//  if(last_state != state) {
//    last_state = state;
//    Serial.println(state, HEX);
//  Serial.println(1, HEX);  
//    if(state == HIGH)
//worker();
//delay(100);
//  }
  SPI.transfer(123);
}

