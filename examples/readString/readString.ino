#include "GPS.h"

GPS gps(3, 4);
// GPS(rx, tx)
// rx pin
// tx pin

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gps.begin();
  Serial.println();
  Serial.println("---- START OF MESSAGE ----");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(gps.readString());
}
