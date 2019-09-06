#include "GPS.h"

GPS gps(3); 
// GPS(rx, length)
// rx = rx pin
// length(optional) = message maximum length, default 800;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gps.begin(9600);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println();
  Serial.println("---- START OF MESSAGE ----");
  Serial.println();
  Serial.println(gps.readString());
  Serial.println();
  Serial.println("----- END OF MESSAGE -----");
  Serial.println();
  delay(100);
}
