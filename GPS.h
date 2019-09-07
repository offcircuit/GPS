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

    String print(String data) {
      _serial->println(char(0x24) + data + char(0x2A) + String(checksum(data), HEX));
      return readString();
    }

    String readString(uint16_t length = 0) {
      String data = "";
      uint16_t exit = 0;
        if (length) do while (_serial->available()) data += char(_serial->read()); while ((data.length() < length) && (exit++ < 0xFFFF));
        else do while (_serial->available()) data += char(_serial->read()); while (length++ < 0xFFFF);
      return data;
    }

    String write(char* data, uint8_t length) {
      _serial->write(data, length);
      return readString();
    }

};

#endif
