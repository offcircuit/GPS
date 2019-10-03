#include "GPS.h"

GPS::GPS(uint8_t rx, uint8_t tx) {
  _serial = new SoftwareSerial(rx, tx);
}

uint32_t GPS::begin(uint32_t speed) {
  uint32_t rate = baud();
  if (speed && (speed != rate)) return setBaud(speed);
  return rate;
}

uint32_t GPS::baud() {
  const uint16_t map[12] = {768, 384, 192, 96, 48, 32, 16, 8, 4, 2, 1, 0};
  uint8_t index = 0;
  _serial->setTimeout(100);
  do {
    _serial->begin(map[index] * 1200UL);
    print(String(GPS_PUBX));
  }
  while (!find(GPS_GPTXT).length() && (11 > ++index));
  _serial->setTimeout(1000);
  return map[index] * 1200UL;
}

String GPS::find(char *buffer) {
  if (_serial->find(buffer)) {
    String data = String(buffer);
    uint8_t c = 0, n = 0;
    for (uint8_t i = data.indexOf(char(0x24)) + 1; i < data.length(); i++) n ^= uint8_t(data[i]);
    do if (_serial->available()) {
        n ^= c;
        c = _serial->read();
        data += char(c);
      } while (c != 0x2A);
    c = data.length();
    while (_serial->available() < 2);
    data += char(_serial->read());
    data += char(_serial->read());
    if (prefix(n, HEX).equalsIgnoreCase(data.substring(c, c + 2))) return data;
  }
  return "";
}

String GPS::getDateTime() {
  print(String(GPS_PUBX) + prefix(GPS_DATETIME, DEC));
  return find(GPS_PUBX);
}

String GPS::getGeolocation() {
  print(String(GPS_PUBX) + prefix(GPS_GEOLOCATION, DEC));
  return find(GPS_PUBX);
}

String GPS::getSatellites() {
  print(String(GPS_PUBX) + prefix(GPS_SATELLITES, DEC));
  return find(GPS_PUBX);
}

String GPS::prefix(uint8_t val, uint8_t base) {
  return String(val / base, base) + String(val % base, base);
}

void GPS::print(String data) {
  uint8_t n = 0;
  for (uint8_t i = 1; i < data.length(); i++) n ^= uint8_t(data[i]);
  _serial->println(String(char(0x24)) + data + String(char(0x2A)) + prefix(n, HEX));
}

String GPS::read() {
  String data;
  uint8_t h = 0, l = 0;
  uint16_t length;
  _serial->findUntil("µ", "b");

  while (_serial->available() < 4);
  l += (h += _serial->read());
  l += (h += _serial->read());
  length = _serial->read() | _serial->read() << 8;
  l += (h += length & 0xFF);
  l += (h += length >> 8);

  do if (_serial->available()) {
      uint8_t c = _serial->read();
      data += char(c);
      l += (h += c);
      length--;
    } while (length);

  while (_serial->available() < 2);
  if (uint8_t(_serial->read()) == h && uint8_t(_serial->read()) == l) return data;
}

String GPS::readString() {
  return find("$");
}

bool GPS::reset(uint16_t mode) {
  uint8_t buffer[10] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, mode & 0xFF, mode >> 8, 0x02, 0x00};
  write(buffer, 10);
  if (_serial->find(strcat(GPS_GPTXT, "01,01,02,"))) return read();
}

uint32_t GPS::setBaud(uint32_t speed) {
  print(String(GPS_PUBX) + "41,1,0007,0003," + String(speed) + ",0");
  return baud();
}

String GPS::version() {
  uint8_t buffer[6] = {0xB5, 0x62, 0x0A, 0x04, 0x00, 0x00};
  write(buffer, 6);
  return read();
}

void GPS::write(uint8_t *data, size_t length) {
  uint8_t h = 0, l = 0;
  for (size_t i = 2; i < length; i++) l += (h += data[i]);
  _serial->write(data, length);
  _serial->write(h);
  _serial->write(l);
}
