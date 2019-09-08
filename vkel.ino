#include "VKEL.h"

GPS gps(2, 3);

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

  /*gps.read();
  Serial.print(gps.hour);
  Serial.print(":");
  Serial.print(gps.minute);
  Serial.print(":");
  Serial.print(gps.second);
  Serial.print(".");
  Serial.print(gps.dsec);
  Serial.print(" - ");
  Serial.print(gps.day);
  Serial.print("/");
  Serial.print(gps.month);
  Serial.print("/");
  Serial.println(gps.year);
  Serial.print("latitude: ");
  Serial.print(gps.latitude, 6);
  Serial.println(gps.NS ? " S" : " N");
  Serial.print("longitude: ");
  Serial.print(gps.longitude, 6);
  Serial.println(gps.WE ? " E" : " W");*/
  Serial.println();
  Serial.println("----- END OF MESSAGE -----");
  Serial.println();
  delay(1000);
}
