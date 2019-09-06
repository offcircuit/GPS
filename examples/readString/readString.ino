#include "GPS.h"
#include "NMEA.h"

GPS gps(3);

String s;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gps.begin(9600);
  delay(500);
}

void loop() {
  Serial.println("********************");

   /*Serial.println(gps.readString());*/

 NMEA nmea;
    GPRMC rmc = nmea.readRMC(gps.readString());

    //Serial.print(rmc.day); Serial.print("/"); Serial.print(rmc.month); Serial.print("/"); Serial.print(rmc.year);
    //Serial.print(" - ");
    Serial.print(rmc.hour); Serial.print(":"); Serial.print(rmc.minute); Serial.print(":"); Serial.print(rmc.second); Serial.print(".");  Serial.println(rmc.millis);

    Serial.print("Latitude: ");  Serial.print(rmc.latitude, 6); Serial.println(rmc.NS ? " S" : " N");
    Serial.print("Longitude: "); Serial.print(rmc.longitude, 6); Serial.println(rmc.WE ? " E" : " W");
  
  Serial.println("------------------------");
  //delay(2000);
}
