#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>

class GPS {
  private:
    uint16_t _length;

  public:
    SoftwareSerial *_serial;

    GPS(uint8_t rx, uint16_t length = 800): _length(length) {
      _serial = new SoftwareSerial(rx, rx);
    }

    void begin(uint32_t baud) {
      _serial->begin(baud);
    }

    String readString() {
      String data = "$";
      uint16_t t = _length;
      while (!_serial->available() || (_serial->read() != 0x24));

      do if (_serial->available() && (data += char(_serial->read())) && t--); while (t);

      data.remove(0, data.indexOf(char(0x0D)));
      data.remove(data.lastIndexOf("*") + 3);
      return data;
    }
};

#endif
