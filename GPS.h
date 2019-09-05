#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>

struct GPGGA {
  uint8_t hour, minute, second;
  uint16_t millis;
  float latitude, longitude;
  bool NS, WE;
  uint8_t fix;
  uint8_t satellites;
  float HDPO;
  float altitude;
  char altitude_u;
  float geoidal;
  char geoidal_u;
  uint16_t age;
  uint16_t station;
  bool checksum;
};

struct GPGLL {
  float latitude, longitude;
  bool NS, WE;
  uint8_t hour, minute, second;
  uint16_t millis;
  char status;
  char mode;
  float altitude;
  bool checksum;
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
  uint8_t hour, minute, second;
  uint16_t millis;
  char status;
  float latitude, longitude;
  bool NS, WE;
  float speed;
  float course;
  uint8_t day, month, year;
  float variation;
  char direction;
  char mode;
  bool checksum;
};

struct GPVTG {
  float TCourse;
  char TReference;
  float MCourse;
  char MReference;
  float NSpeed;
  char NSpeed_u;
  float KSpeed;
  char KSpeed_u;
  char mode;
  bool checksum;
};

struct GPZDA {
  uint8_t hour, minute, second;
  uint16_t millis;
  uint8_t day, month;
  uint16_t year;
  uint8_t localHour;
  uint8_t localMinute;
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

    bool checksum(String &data) {
      uint8_t n = 0;
      for (uint8_t i = 1; i < data.lastIndexOf("*"); i++) n ^= byte(data[i]);
      data.remove(0, data.indexOf(",") + 1);
      return data.substring(data.lastIndexOf("*") + 1).equalsIgnoreCase(String((n >> 4), HEX) + String((n & 0xF), HEX));
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

        getTime(data, result.hour, result.minute, result.second, result.millis);
        getGeoposition(data, result.latitude, result.NS, result.longitude, result.WE);
        getInt8(data, result.fix);
        getInt8(data, result.satellites);
        getFloat(data, result.HDPO);
        getFloat(data, result.altitude);
        getChar(data, result.altitude_u);
        getFloat(data, result.geoidal);
        getChar(data, result.geoidal_u);
        getInt16(data, result.age);
        getInt16(data, result.station);

        return result;
      }
    }

    GPGLL readGLL() {
      GPGLL result;

      String data = NMEA(readString(), "$GPGLL");

      if (result.checksum = checksum(data)) {

        getGeoposition(data, result.latitude, result.NS, result.longitude, result.WE);
        getTime(data, result.hour, result.minute, result.second, result.millis);
        getChar(data, result.status);
        getChar(data, result.mode);

        return result;
      }
    }

    GPGSA readGSA() {
      GPGSA result;

      String data = NMEA(readString(), "$GPGSA");

      if (result.checksum = checksum(data)) {

        return result;
      }
    }

    GPGSV readGSV() {
      GPGSV result;

      String data = NMEA(readString(), "$GPGSV");

      if (result.checksum = checksum(data)) {

        return result;
      }
    }

    GPRMC readRMC() {
      GPRMC result;

      String data = NMEA(readString(), "$GPRMC");

      if (result.checksum = checksum(data)) {

        getTime(data, result.hour, result.minute, result.second, result.millis);
        getChar(data, result.status);
        getGeoposition(data, result.latitude, result.NS, result.longitude, result.WE);
        getFloat(data, result.speed);
        getFloat(data, result.course);
        getDate(data, result.day, result.month, result.year);
        getFloat(data, result.variation);
        getChar(data, result.direction);
        getChar(data, result.mode);

        return result;
      }
    }

    GPVTG readVTG() {
      GPVTG result;

      String data = NMEA(readString(), "$GPVTG");

      if (result.checksum = checksum(data)) {
        
        getFloat(data, result.TCourse);
        getChar(data, result.TReference);
        getFloat(data, result.MCourse);
        getChar(data, result.MReference);
        getFloat(data, result.NSpeed);
        getChar(data, result.NSpeed_u);
        getFloat(data, result.KSpeed);
        getChar(data, result.KSpeed_u);
        getChar(data, result.mode);

        return result;
      }
    }

    GPZDA readZDA() {
      GPZDA result;

      String data = NMEA(readString(), "$GPZDA");

      if (result.checksum = checksum(data)) {
        
        getTime(data, result.hour, result.minute, result.second, result.millis);
        getInt8(data, result.day);
        getInt8(data, result.month);
        getInt16(data, result.year);
        getInt8(data, result.localHour);
        getInt8(data, result.localMinute);
        
        return result;
      }
    }

  private:
    void getChar(String &data, char &val) {
      val = data.charAt(0);
      data.remove(0, data.indexOf(",") + 1);
    }

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

    void getInt8(String &data, uint8_t &val) {
      val = data.substring(0, data.indexOf(",")).toInt();
      data.remove(0, data.indexOf(",") + 1);
    }

    void getInt16(String &data, uint16_t &val) {
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
