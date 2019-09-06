#ifndef GPS_H
#define GPS_H

#include <SoftwareSerial.h>

class GPS {
  private:
    SoftwareSerial *_serial;
    uint16_t _length;

  public:
    explicit GPS(uint8_t rx, uint16_t length = 800): _length(length) {
      _serial = new SoftwareSerial(rx, rx);
    }

    void begin(uint32_t baud) {_serial->begin(baud);
      _serial->begin(9600);
    }

    String readString() {
      String data = String(0x24);
      uint16_t lenght = _length;
      while (!_serial->available() || (_serial->read() != 0x24));
      do if (_serial->available() && (data += char(_serial->read())) && lenght--); while (lenght);
      data.remove(0, data.indexOf(char(0x24)));
      data.remove(data.lastIndexOf(char(0x2A)) + 3);
      data.trim();
      return data;
    }

    void sendCommand(String command) {
      _serial->println(command);
    }

};

#endif
