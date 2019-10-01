#ifndef GPS_H
#define GPS_H

#include <sys/types.h>
#include <SoftwareSerial.h>

#define GPS_NEMA_PUBX "PUBX,"
#define GPS_NEMA_TXT "GPTXT,"

#define GPS_PUBX_GEOLOCATION 0
#define GPS_PUBX_SATELLITES 3
#define GPS_PUBX_DATETIME 4

#define GPS_HOTSTART 0x0000
#define GPS_WARMSTART 0x0001
#define GPS_COLDSTART 0xffff

class GPS {
  private:
    SoftwareSerial *_serial;
    uint32_t baud();
    uint16_t checksum(uint8_t *data, uint8_t length);
    String prefix(uint8_t data, uint8_t base);
    void send(String data);

  public:
    GPS(uint8_t rx, uint8_t tx);
    uint32_t begin(uint32_t speed = 0);
    String getGeoposition();
    String getSatellites();
    String getDateTime();
    String print(String data);
    uint8_t reset(uint16_t mode = 0);
    uint32_t setBaud(uint32_t speed);
    String version();
    String readString();
    void write(uint8_t *data, uint8_t length);
};

#endif
