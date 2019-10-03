#ifndef GPS_H
#define GPS_H

#include <sys/types.h>
#include <SoftwareSerial.h>

#define GPS_GPTXT "$GPTXT,"
#define GPS_PUBX "$PUBX,"

#define GPS_GEOLOCATION 0
#define GPS_SATELLITES 3
#define GPS_DATETIME 4

#define GPS_HOTSTART 0x0000
#define GPS_WARMSTART 0x0001

class GPS {
  private:
    SoftwareSerial *_serial;
    uint32_t baud();
    String prefix(uint8_t val, uint8_t base);

  public:
    GPS(uint8_t rx, uint8_t tx);
    uint32_t begin(uint32_t speed = 0);
    String find(char *buffer);
    String getDateTime();
    String getGeolocation();
    String getSatellites();
    void print(String data);
    String read();
    String readString();
    bool reset(uint16_t mode = GPS_HOTSTART);
    uint32_t setBaud(uint32_t speed);
    String version();
    void write(uint8_t *data, size_t length);
};

#endif
