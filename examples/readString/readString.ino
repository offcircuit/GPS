#include "GPS.h"

GPS gps(3, 4);
// GPS(rx, tx)
// rx = rx pin
// tx = tx pin

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gps.begin();
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
