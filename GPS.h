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

class GPS {
  private:
    uint32_t baud();
    String prefix(uint8_t data, uint8_t base);
    void send(String data);

  public:
    SoftwareSerial *_serial;
    GPS(uint8_t rx, uint8_t tx);
    uint32_t begin(uint32_t speed = 0);
    String getDateTime();
    String getGeoposition();
    String getSatellites();
    String print(uint8_t data);
    bool reset(uint16_t mode = GPS_HOTSTART);
    uint32_t setBaud(uint32_t speed);
    String readString();
    String version();
    String write(uint8_t *data, uint8_t length);
};

#endif
