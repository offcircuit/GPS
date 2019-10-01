#include "GPS.h"

GPS gps(4, 5);

void setup() {
  Serial.begin(115200);
  Serial.println();
  uint32_t rate;
  while (!(rate = gps.begin()));
  Serial.println("...DateTime...");
  Serial.println();
  Serial.println(gps.getDateTime());
  Serial.println();
  Serial.println("...Geoposition...");
  Serial.println();
}

void loop() {
  Serial.println(gps.getGeoposition());
}
