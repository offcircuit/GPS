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
    send(String(GPS_NEMA_PUBX));
  } while (!_serial->find(GPS_NEMA_TXT) && (11 > ++index));
  _serial->setTimeout(1000);
  return map[index] * 1200UL;
}

uint16_t GPS::checksum(uint8_t *data, uint8_t length) {
  uint8_t h = 0, l = 0;
  for (uint8_t i = 0; i < length; i++) l += (h += data[i]);
  return h << 8 | l;
}

String GPS::getDateTime() {
  return print(String(GPS_NEMA_PUBX) + prefix(GPS_PUBX_DATETIME, DEC));
}

String GPS::getGeoposition() {
  return print(String(GPS_NEMA_PUBX) + prefix(GPS_PUBX_GEOLOCATION, DEC));
}

String GPS::getSatellites() {
  return print(String(GPS_NEMA_PUBX) + prefix(GPS_PUBX_SATELLITES, DEC));
}

String GPS::prefix(uint8_t data, uint8_t base) {
  return String(data / base, base) + String(data % base, base);
}

String GPS::print(String data) {
  send(data);
  if (_serial->find(GPS_NEMA_PUBX)) return _serial->readStringUntil(char(0x0A));
}

String GPS::readString() {
  while (!_serial->available());
  String s = _serial->readStringUntil(char(0x0D));
  s.trim();
  return s;
}

uint8_t GPS::reset(uint16_t mode) {
  uint8_t data[8] = {0x06, 0x04, 0x04, 0x00, mode >> 8, mode & 0xFF, 0x02, 0x00};
  write(data, 8);
  return _serial->find(GPS_NEMA_TXT);
}

void GPS::send(String data) {
  uint8_t n = 0;
  for (uint8_t i = 0; i < data.length(); i++) n ^= byte(data[i]);
  _serial->println(String(char(0x24)) + data + String(char(0x2A)) + prefix(n, HEX));
}

uint32_t GPS::setBaud(uint32_t speed) {
  Serial.println("set baud ......");
  send(String(GPS_NEMA_PUBX) + "41,1,0007,0003," + String(speed) + ",0");
  return baud();
}

String GPS::version() {
  uint8_t data[4] = {0x0A, 0x04, 0x00, 0x00};
  char e[2] = {0x0a, 0x04};
  write(data, 4);
  _serial->findUntil(e, "\n");
  String s = _serial->readStringUntil(char(0x24));
  s.remove(s.length() - 2, 2);
  return s;
}

void GPS::write(uint8_t *data, uint8_t length) {
  uint16_t r = checksum(data, length);
  _serial->write(0xB5);
  _serial->write(0x62);
  _serial->write(data, length);
  _serial->write(r >> 8);
  _serial->write(r & 0xFF);
}
