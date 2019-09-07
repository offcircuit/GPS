#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>

#define CMD_RESET

class GPS {
  public:
    SoftwareSerial *_serial;

    GPS(uint8_t rx, uint8_t tx) {
      _serial = new SoftwareSerial(rx, tx);
    }

    void begin() {
      _serial->begin(9600);
    }

    uint8_t checksum(String data) {
      uint8_t n = 0;
      for (uint8_t i = 0; i < data.length(); i++) n ^= byte(data[i]);
      return n;
    }

    String readString() {
      String data;
      uint16_t length = 800;
      do if (_serial->available() && (data += char(_serial->read())) && length--); while (length);
      return data;
    }

    String write(char* data, uint8_t length) {
      _serial->write(data, length);
      return readString();
    }
};

#endif
