#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <TM1637Display.h>
#include <wifi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>

#define CLK D6
#define DIO D5

// COMP-10184 – Mohawk College
// Advanced Sensors
//
// This program...
//
// @author Chris Dent
// @id 000849456
//
// I created this work and I have not shared it with anyone;

TM1637Display display(CLK, DIO);
Adafruit_AHTX0 aht;
WiFiUDP ntpUDP;
const long offSet = -4 * 60 * 60;
NTPClient timeClient(ntpUDP, "0.ca.pool.ntp.org", offSet);

bool changed;
int lastState = 1;
int displayOption;

void showTemp(sensors_event_t temp){
  if (changed){
    display.clear();
    changed = false;
    display.setSegments((const uint8_t[]){0x00}, 1, 1);
    display.setSegments((const uint8_t[]){0x63}, 2, 3);
  }

  display.showNumberDec(temp.temperature, false, 2, 1);
}

void showHumidity(sensors_event_t humidity){
  if(changed) {
    display.clear();
    changed = false;
  }
  display.showNumberDecEx(humidity.relative_humidity, 0x40, false, 2, 0);
}
// I'm sorry this doesnt follow convetions, I thought it was a great pun...one might say, punny...
void itsSHOWTIME(String time){
  Serial.println(time);
}

void cycle(sensors_event_t humidity, sensors_event_t temp){
  display.clear();
  itsSHOWTIME(timeClient.getFormattedTime());
  delay(2000);
  display.clear();
  changed = true;

  showTemp(temp);
  delay(2000);
  display.clear();
  changed = true;

  showHumidity(humidity);
  delay(2000);
  display.clear();
  changed = true;

}

void showDashes() {
  
}

void setup() {
  displayOption = 0;
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    display.clear();
    delay(500);
    display.setSegments((const uint8_t[]){0x40, 0x40, 0x40, 0x40});
    delay(500);
  }

  timeClient.begin();
  

  if (!aht.begin()) {
    Serial.println("Cannot find AHT");
    while (1) delay(10);
  }
  // Clear display of any remnants
  display.clear();
  display.setBrightness(0x0a); 
  pinMode(D7, INPUT_PULLUP);
}

void loop() {
  timeClient.update();
  // checks to see if the button is constant press
  if (digitalRead(D7) == 1){  
    lastState = digitalRead(D7);
  }
// sensors_event_t is a C struct
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);

  switch (displayOption)
  {
    case 0:
      itsSHOWTIME(timeClient.getFormattedTime());
      break;
  
    case 1:
      showHumidity(humidity);
      break;

    case 2:
      showTemp(temp);
      break;

    case 3:
      cycle(humidity, temp);
      break;
  }

  if (digitalRead(D7) == 0 && digitalRead(D7) != lastState){
    lastState = digitalRead(D7);
    changed = true;
    if (displayOption == 3){
      displayOption = 0;
      Serial.println(String(displayOption));
    }
    else{
      displayOption++;
      Serial.println(String(displayOption));
    }
  }
  delay(50);
}
