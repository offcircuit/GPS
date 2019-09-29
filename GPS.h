#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>
#include <Arduino.h>

class GPS {
  public:
    SoftwareSerial *_serial;
    String _data;

    GPS(uint8_t rx, uint8_t tx) {
      _serial = new SoftwareSerial(rx, tx);
    }

    uint32_t begin(uint32_t speed = 0) {
      uint32_t rate = baud();
      if (speed && (speed != rate)) return setBaud(speed);
      return rate;
    }

    uint32_t baud() {
      const uint16_t map[12] = {768, 384, 192, 96, 48, 32, 16, 8, 4, 2, 1, 0};
      uint8_t index = 0;
      do _serial->begin(map[index] * 1200UL);
      while (!_serial->findUntil("$", "/n") && (11 > ++index));
      return map[index] * 1200UL;
    }

    uint32_t setBaud(uint32_t speed) {
      print("PUBX,41,1,0007,0003," + String(speed) + ",0");
      delay(1000);
      Serial.println("setting baud ...");
      return baud();
    }

    uint32_t time() {
    }

    uint16_t checksum(uint8_t *buffer, size_t length) {
      uint8_t h = 0, l = 0;
      for (size_t i = 0; i < length; i++) {
        h = h + buffer[i];
        l = l + h;
      }
      return h << 8 | l;
    }

    void write(uint8_t *data, uint8_t length) {
      uint16_t r = checksum(data, length);
      _serial->write(0xB5);
      _serial->write(0x62);
      _serial->write(data, length);
      _serial->write(r >> 8);
      _serial->write(r & 0xFF);
    }


    bool reset(uint16_t mode = 0) {
      uint8_t data[8] = {0x06, 0x04, 0x04, 0x00, mode >> 8, mode & 0xFF, 0x02, 0x00};
      write(data, 8);
    }

    bool version() {
      uint8_t data[4] = {0x0A, 0x04, 0x00, 0x00};
      write(data, 4);
    }

    bool protocol() {
      uint8_t data[4] = {0x06, 0x00, 0x00, 0x00};
      print("PUBX,00");
      // _serial->println("$PUBX,06,,0*5F");
      // write(data, 4);
    }

    String dateTime() {
      print("PUBX,04");
      return readINEMA("$PUBX");
    }

    String getGeoposition() {
      print("PUBX,00");
      return readINEMA("$PUBX");
    }


    uint8_t checksum(String data) {
      uint8_t n = 0;
      for (uint8_t i = 0; i < data.length(); i++) n ^= byte(data[i]);
      return n;
    }

    void print(String data) {
      Serial.println("----------------");
      Serial.println(char(0x24) + data + char(0x2A) + String(checksum(data), HEX));
      Serial.println("----------------");
      _serial->println(char(0x24) + data + char(0x2A) + String(checksum(data), HEX));
    }

    String readINEMA(String INEMA) {
      String data;
      uint8_t attempts = 255;
      while (attempts-- && String(data = readString()).substring(0, INEMA.length()) != INEMA);
      return attempts ? data : "";
    }

    String readString() {
      String data = "$";
      while (_serial->available() && (_serial->read() != char(0x24)));
      data += _serial->readStringUntil(char(0x0D));
      data = data.substring(data.lastIndexOf(char(0x24)));
      data.trim();

      return data;
    }

};

#endif
