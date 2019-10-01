#include "gps.h"

GPS gps(4, 5);

void setup() {
  Serial.begin(115200);
  Serial.println();
  uint32_t rate;
  while (!(rate = gps.begin(19200)));
  Serial.println(rate);

  Serial.println("......");
  Serial.println(gps.getGeoposition());
  Serial.println(gps.getSatellites());
  Serial.println(gps.getDateTime());
  Serial.println(gps.version());
  Serial.println(gps.reset(0x01));
  Serial.println("......");
}

void loop() {
  /*Serial.println(gps.read()); */
  Serial.println(gps.getGeoposition());
}
