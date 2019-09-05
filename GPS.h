#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>

struct GPGGA {
  char altitude_unit, geoidal_unit;
  uint8_t hour, minute, second;
  uint16_t millis;
  uint8_t indicator, satellites;
  float latitude, longitude, altitude;
  float HDPO, geoidal;
  bool NS, WE, checksum;
  uint16_t age, station;
};

struct GPGLL {
  char status, mode;
  uint8_t hour, minute, second;
  uint16_t millis;
  float latitude, longitude, altitude;
  bool NS, WE, checksum;
};

struct GPGSA {
  uint8_t mode_1;
  char mode_2;
  float PDOP, HDOP;
  bool checksum;
};

struct GPGSV {
  uint8_t messages, msg_id, satellites, sat_id, elevation;
  uint16_t azinmuth;
  bool checksum;
};

struct GPRMC {
  char status, mode, direction;
  uint8_t year, month, day, hour, minute, second;
  uint16_t millis;
  float latitude, longitude;
  float speed, course, variation;
  bool NS, WE, checksum;
};

struct GPVTG {
  char uints, mode;
  float Tcourse, Mcourse, speed;
  bool Treference, Mreference, checksum;

};

struct GPZDA {
  uint8_t hour, minute, second, day, month;
  uint16_t year, millis, hours, minutes;
  bool checksum;
};

class GPS {
  public:
    SoftwareSerial *_serial;

    GPS(uint8_t rx, uint8_t tx) {
      _serial = new SoftwareSerial(rx, tx);
    }

    void begin(uint32_t baud) {
      _serial->begin(baud);
    }

    String readString() {
      String data;
      uint16_t time = millis();

      do if (_serial->available()) while (_serial->available()) data += char(_serial->read());
      while (millis() < (time + 1000));
      return data;
    }

    bool checksum(String data) {
      uint8_t n = 0;
      for (uint8_t i = 1; i < data.lastIndexOf("*"); i++) n ^= byte(data[i]);
      return data.substring(data.lastIndexOf("*") + 1).equalsIgnoreCase(String(n, HEX));
    }

    String NMEA(String data, String record) {
      data = data.substring(data.lastIndexOf(record), data.indexOf("$", data.lastIndexOf(record) + 1));
      data.trim();
      return data;
    }

    GPGGA readGGA() {
      GPGGA result;

      String data = NMEA(readString(), "$GPGGA");

      if (result.checksum = checksum(data)) {

        data.remove(0, data.indexOf(",") + 1);

        getTime(data, result.hour, result.minute, result.second, result.millis);
        getGeoposition(data, result.latitude, result.NS, result.longitude, result.WE);

        result.indicator = data.substring(0, data.indexOf(",")).toInt();
        data.remove(0, data.indexOf(",") + 1);

        result.satellites = data.substring(0, data.indexOf(",")).toInt();
        data.remove(0, data.indexOf(",") + 1);

        getFloat(data, result.HDPO);

        getFloat(data, result.altitude);

        result.altitude_unit = data.charAt(0);
        data.remove(0, data.indexOf(",") + 1);

        getFloat(data, result.geoidal);

        result.geoidal_unit = data.charAt(0);
        data.remove(0, data.indexOf(",") + 1);

        result.age = data.substring(0, data.indexOf(",")).toInt();
        data.remove(0, data.indexOf(",") + 1);

        result.station = data.substring(0, data.indexOf(",")).toInt();
        data.remove(0, data.indexOf(",") + 1);

        return result;
      }
    }

    GPGLL readGLL() {
      GPGLL result;

      String data = NMEA(readString(), "$GPGLL");

      if (result.checksum = checksum(data)) {

        data.remove(0, data.indexOf(",") + 1);

        getGeoposition(data, result.latitude, result.NS, result.longitude, result.WE);
        getTime(data, result.hour, result.minute, result.second, result.millis);

        result.status = data.charAt(0);
        data.remove(0, data.indexOf(",") + 1);

        result.mode = data.charAt(0);
      }
    }

    GPGSA readGSA() {
      GPGSA result;

      String data = NMEA(readString(), "$GPGSA");

      if (result.checksum = checksum(data)) {

        data.remove(0, data.indexOf(",") + 1);
      }
    }

    GPGSV readGSV() {
      GPGSV result;

      String data = NMEA(readString(), "$GPGSV");

      if (result.checksum = checksum(data)) {

        data.remove(0, data.indexOf(",") + 1);
      }
    }

    GPRMC readRMC() {
      GPRMC result;

      String data = NMEA(readString(), "$GPRMC");

      if (result.checksum = checksum(data)) {

        data.remove(0, data.indexOf(",") + 1);

        getTime(data, result.hour, result.minute, result.second, result.millis);

        result.status = data.charAt(0);
        data.remove(0, data.indexOf(",") + 1);

        getGeoposition(data, result.latitude, result.NS, result.longitude, result.WE);

        getFloat(data, result.speed);
        getFloat(data, result.course);

        getDate(data, result.day, result.month, result.year);

        getFloat(data, result.variation);
        result.direction = data.charAt(0);
        data.remove(0, data.indexOf(",") + 1);

        result.mode = data.charAt(0);

        return result;
      }
    }

    GPVTG readVTG() {
      GPVTG result;

      String data = NMEA(readString(), "$GPVTG");

      if (result.checksum = checksum(data)) {

        data.remove(0, data.indexOf(",") + 1);
      }
    }

    GPZDA readZDA() {
      GPGSA result;

      String data = NMEA(readString(), "$GPZDA");

      if (result.checksum = checksum(data)) {

        data.remove(0, data.indexOf(",") + 1);
      }
    }

  private:
    void getDate(String &data, uint8_t &day, uint8_t &month, uint8_t &year) {
      day = data.substring(0, 2).toInt();
      month = data.substring(2, 4).toInt();
      year = data.substring(4, 6).toInt();
      data.remove(0, data.indexOf(",") + 1);
    }

    void getFloat(String &data, float &val) {
      val = data.substring(0, data.indexOf(",")).toFloat() / 100;
      data.remove(0, data.indexOf(",") + 1);
    }

    void getGeoposition(String &data, float &latitude, bool &ns, float &longitude, bool &we) {
      getFloat(data, latitude);
      ns = data.substring(0, data.indexOf(",")) == "S";
      data.remove(0, data.indexOf(",") + 1);

      getFloat(data, longitude);
      we = data.substring(0, data.indexOf(",")) == "E";
      data.remove(0, data.indexOf(",") + 1);
    }

    void getInt(String &data, uint8_t &val) {
      val = data.substring(0, data.indexOf(",")).toInt();
      data.remove(0, data.indexOf(",") + 1);
    }

    void getTime(String &data, uint8_t &hour, uint8_t &minute, uint8_t &second, uint16_t &millis) {
      hour = data.substring(0, 2).toInt();
      minute = data.substring(2, 4).toInt();
      second = data.substring(4, 6).toInt();
      millis = data.substring(7, 10).toInt();
      data.remove(0, data.indexOf(",") + 1);
    }
};

#endif
