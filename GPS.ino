#include "gps.h"

GPS gps(4, 5);

void setup() {
  Serial.begin(115200);
  Serial.println();
  uint32_t rate;
  while (!(rate = gps.begin()));
  Serial.println(rate);

  Serial.println("......");
  Serial.println(gps.getGeoposition());
  Serial.println(gps.getSatellites());
  Serial.println(gps.getDateTime());
  Serial.println(gps.version());/**/
  /*
    String s = gps.info();
    for (uint8_t i = 0; i < s.length(); i++)
    {
      Serial.print(s[i], HEX);
      Serial.print(" ");
    }
  */
  Serial.println();
  Serial.println("......");
}

void loop() {
  /* Serial.println(gps.readString()); */
}
