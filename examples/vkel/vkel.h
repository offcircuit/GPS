#ifndef VKEL_H
#define VKEL_H

#include "GPS.h"

class VKEL: GPS {
  public:
    uint8_t hour, minute, second, day, month, year;
    uint16_t csec;
    float latitude, longitude;
    bool NS, WE;

    VKEL(uint8_t rx, uint8_t tx): GPS(rx, tx) {}

    void begin() {
      GPS::begin();
      disable();
    }

    void disable() {
      print("PUBX,40,GGA,1,0,0,0,0,0");
      print("PUBX,40,GLL,1,0,0,0,0,0");
      print("PUBX,40,GSA,1,0,0,0,0,0");
      print("PUBX,40,GSV,1,0,0,0,0,0");
      print("PUBX,40,RMC,1,0,0,0,0,0");
      print("PUBX,40,VTG,1,0,0,0,0,0");
    }

    bool read() {
      String data;
      print("PUBX,04");
      data = readString();
      data.remove(0, data.indexOf(",") + 1);
      data.remove(0, data.indexOf(",") + 1);
      getTime(data, hour, minute, second, csec);
      getDate(data, day, month, year);
      print("PUBX,00");
      data = readString();
      data.remove(0, data.indexOf(",") + 1);
      data.remove(0, data.indexOf(",") + 1);
      data.remove(0, data.indexOf(",") + 1);
      getGeoposition(data, latitude, NS, longitude, WE);
      Serial.println();
    }

  private:
    void getDate(String &data, uint8_t &day, uint8_t &month, uint8_t &year) {
      day = data.substring(0, 2).toInt();
      month = data.substring(2, 4).toInt();
      year = data.substring(4, 6).toInt();
      data.remove(0, data.indexOf(",") + 1);
    }

    void getGeoposition(String &data, float &latitude, bool &ns, float &longitude, bool &we) {
      latitude = data.substring(0, data.indexOf(",")).toFloat() / 100;
      data.remove(0, data.indexOf(",") + 1);
      ns = data.charAt(0) == "S";
      data.remove(0, data.indexOf(",") + 1);

      longitude = data.substring(0, data.indexOf(",")).toFloat() / 100;
      data.remove(0, data.indexOf(",") + 1);
      we = data.charAt(0) == "E";
      data.remove(0, data.indexOf(",") + 1);
    }

    void getTime(String &data, uint8_t &hour, uint8_t &minute, uint8_t &second, uint16_t &csec) {
      hour = data.substring(0, 2).toInt();
      minute = data.substring(2, 4).toInt();
      second = data.substring(4, 6).toInt();
      csec = data.substring(7, 9).toInt();
      data.remove(0, data.indexOf(",") + 1);
    }
};

#endif
