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
    send(String(GPS_PUBX));
  } while (!_serial->find(GPS_GPTXT) && (11 > ++index));
  _serial->readStringUntil(char(0x0A));
  _serial->setTimeout(1000);
  return map[index] * 1200UL;
}

String GPS::getDateTime() {
  return print(GPS_DATETIME);
}

String GPS::getGeoposition() {
  return print(GPS_GEOLOCATION);
}

String GPS::getSatellites() {
  return print(GPS_SATELLITES);
}

String GPS::prefix(uint8_t data, uint8_t base) {
  return String(data / base, base) + String(data % base, base);
}

String GPS::print(uint8_t data) {
  return print(String(GPS_PUBX) + prefix(data, DEC), GPS_PUBX);
}

String GPS::print(String data, char *nema) {
  send(data);
  if (_serial->find(nema)) return _serial->readStringUntil(char(0x0A));
}

String GPS::readString() {
  return _serial->readStringUntil(char(0x0A));
}

bool GPS::reset(uint16_t mode) {
  uint8_t data[8] = {0x06, 0x04, 0x04, 0x00, mode >> 8, mode & 0xFF, 0x02, 0x00};
  sendCommand(data, 8);
  if (_serial->find(strcat(GPS_GPTXT, "01,01,02,"))) return read(); 
}

void GPS::send(String data) {
  uint8_t n = 0;
  for (uint8_t i = 0; i < data.length(); i++) n ^= uint8_t(data[i]);
  _serial->println(String(char(0x24)) + data + String(char(0x2A)) + prefix(n, HEX));
}

void GPS::sendCommand(uint8_t *data, size_t length) {
  uint8_t h = 0, l = 0;
  for (uint8_t i = 0; i < length; i++) l += (h += data[i]);
  _serial->write(0xB5);
  _serial->write(0x62);
  _serial->write(data, length);
  _serial->write(h);
  _serial->write(l);
}

uint32_t GPS::setBaud(uint32_t speed) {
  send(String(GPS_PUBX) + "41,1,0007,0003," + String(speed) + ",0");
  return baud();
}

String GPS::version() {
  uint8_t data[4] = {0x0A, 0x04, 0x00, 0x00};
  write(data, 4);
  return read();
}

void GPS::write(uint8_t *data, size_t length) {
  sendCommand(data, length);
}

String GPS::read() {
  String s;
  uint8_t h, l;
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
      s += char(c);
      l += (h += c);
      length--;
    } while (length);
    
  while (_serial->available() < 2);
  if (uint8_t(_serial->read()) == h && uint8_t(_serial->read()) == l) return s;
}
