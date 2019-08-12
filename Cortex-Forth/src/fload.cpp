// Adafruit SPI Flash FatFs Simple Datalogging Example
// Author: Tony DiCola
//
// Fri Aug  9 19:12:29 UTC 2019
// Modified: wa1tnr, 9 August 2019

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>

#define FILE_NAME      "/forth/ascii_xfer_a001.txt"

#if defined(__SAMD51__) || defined(NRF52840_XXAA)
  Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
#else
  #if (SPI_INTERFACES_COUNT == 1)
    Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
  #else
    Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
  #endif
#endif

Adafruit_SPIFlash flash(&flashTransport);
FatFileSystem fatfs;

File thisFile;

void fl_setup(void) {
  Serial.begin(38400);
  while (!Serial);
  Serial.println("shred abn-473-fl\n");
  // Serial.println("using SPI Flash FatFs file load - the 'fload' word");

  if (!flash.begin()) {
    Serial.println("Error");
    while(1);
  }
  Serial.print("Flash chip JEDEC ID: 0x"); Serial.println(flash.getJEDECID(), HEX);

  if (!fatfs.begin(&flash)) {
    Serial.println("Error");
    while(1);
  }
  Serial.println("Mounted filesystem ok");
  Serial.print("  fixed filename:  ");
  Serial.print(FILE_NAME);
  Serial.println("   ... resident on flashROM filesystem\n");

  File dataFile = fatfs.open(FILE_NAME, FILE_READ);
  thisFile = (File) dataFile;
  // Serial.println("READING now omitted - was 'has already run.' 10 Aug 16:42z");
}
