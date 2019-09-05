#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>

struct GPGGA {
  char altitude_unit, geoidal_unit;
  uint8_t year, month, day, hour, minute, second, millis;
  uint8_t indicator, satellites;
  float latitude, longitude, altitude;
  float HDPO, geoidal;
  bool NS, WE, checksum;
  uint16_t age, station;
};

struct GPGLL {
  
};

struct GPGSA {
  
};

struct GPGSV {
  
};

struct GPRMC {
  char status, mode, direction;
  uint8_t year, month, day, hour, minute, second, millis;
  float latitude, longitude;
  float speed, course, variation;
  bool NS, WE, checksum;
};

struct VTG {
  
};

struct ZDA {
  
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

    uint8_t checksum(String data) {
      uint8_t n = 0;
      for (uint8_t i = 1; i < data.lastIndexOf("*"); i++) n ^= byte(data[i]);
      return n;
    }

    String NMEA(String data, String record) {
      data = data.substring(data.lastIndexOf(record), data.indexOf("$", data.lastIndexOf(record) + 1));
      data.trim();
      return data;
    }

    GPRMC readRMC() {
      GPRMC result;

      String data = NMEA(readString(), "$GPRMC");

      if (result.checksum = data.substring(data.lastIndexOf("*") + 1).equalsIgnoreCase(String(checksum(data), HEX))) {

        data.remove(0, data.indexOf(",") + 1);
        Serial.println(data);

        result.hour = data.substring(0, 2).toInt(); 
        result.minute = data.substring(2, 4).toInt(); 
        result.second = data.substring(4, 6).toInt();
        result.millis = data.substring(7, 10).toInt();
        data.remove(0, data.indexOf(",") + 1);

        result.status = data.charAt(0);
        data.remove(0, data.indexOf(",") + 1);

        result.latitude = data.substring(0, data.indexOf(",")).toFloat() / 100;
        data.remove(0, data.indexOf(",") + 1);
        result.NS = data.substring(0, data.indexOf(",")) == "S";
        data.remove(0, data.indexOf(",") + 1);

        result.longitude = data.substring(0, data.indexOf(",")).toFloat() / 100;
        data.remove(0, data.indexOf(",") + 1);
        result.WE = data.substring(0, data.indexOf(",")) == "E";
        data.remove(0, data.indexOf(",") + 1);

        result.speed = data.substring(0, data.indexOf(",")).toFloat();
        data.remove(0, data.indexOf(",") + 1);
        result.course = data.substring(0, data.indexOf(",")).toFloat();
        data.remove(0, data.indexOf(",") + 1);

        result.day = data.substring(0, 2).toInt(); 
        result.month = data.substring(2, 4).toInt(); 
        result.year = data.substring(4, 6).toInt();
        data.remove(0, data.indexOf(",") + 1);

        result.variation = data.substring(0, data.indexOf(",")).toFloat();
        data.remove(0, data.indexOf(",") + 1);
        result.direction = data.charAt(0);
        data.remove(0, data.indexOf(",") + 1);

        result.mode = data.charAt(0);
        data.remove(0, data.indexOf(",") + 1);

        return result;
      }
    }
    
    GPGGA readGGA() {
      GPGGA result;

      String data = NMEA(readString(), "$GPGGA");

      if (result.checksum = data.substring(data.lastIndexOf("*") + 1).equalsIgnoreCase(String(checksum(data), HEX))) {

        data.remove(0, data.indexOf(",") + 1);

        return result;
      }
    }
};


#endif
