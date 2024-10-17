#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <TM1637Display.h>

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
bool showTemp;
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
  if (changed){
    display.clear();
    changed = false;
  }
  display.showNumberDecEx(humidity.relative_humidity, 0x40, false, 2, 0);
}

void setup() {
  displayOption = 0;
  Serial.begin(115200);
  if (!aht.begin()) {
  while (1) delay(10);
  }
  // Clear display of any remnants
  display.clear();
  // Set the degrees symbol
  display.setSegments((const uint8_t[]){0x00}, 1, 1);
  display.setSegments((const uint8_t[]){0x63}, 2, 3);
  display.setBrightness(0x0a); // set brightness
  // Set pin mode for button
  pinMode(D7, INPUT_PULLUP);
}

void loop() {
  // checks to see if the button is constant press
  if (digitalRead(D7) == 1){  
    lastState = digitalRead(D7);
  }
// sensors_event_t is a C struct
  sensors_event_t humidity, temp;
// populate temp and humidity objects with fresh data
  aht.getEvent(&humidity, &temp);
// print temp
  Serial.print("Temperature: ");
  Serial.print(temp.temperature); Serial.println(" degrees C");
// print humidity
  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity); Serial.println("% rH");

  // If the show temp variable is set to true, show the temp and if not, show the humidity
  if(showTemp){
  
  }
  else{
   

  }

  // Checks to see if the button was pressed, and stops constant switching with button press
  if (digitalRead(D7) == 0 && digitalRead(D7) != lastState){
    changed = true;
    showTemp = !showTemp;
    lastState = digitalRead(D7);
  }
  delay(50);
}
