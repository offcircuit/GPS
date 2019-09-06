#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>

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

    String readString(uint16_t length = 800) {
      String data = String(0x24);
      while (!_serial->available() || (_serial->read() != 0x24));
      do if (_serial->available() && (data += char(_serial->read())) && length--); while (length);
      data.remove(0, data.indexOf(char(0x24)));
      data.remove(data.lastIndexOf(char(0x2A)) + 3);
      data.trim();
      return data;
    }

    String write(String data) {
      if (_serial->print(char(0x24) + data + char(0x2A) + String(checksum(data), HEX) + char(0x0D) + char(0x0A))) return readString();
      return "";
    }
};

#endif
