// Adafruit SPI Flash FatFs Simple Datalogging Example
// Author: Tony DiCola
//
// Fri Aug  9 19:12:29 UTC 2019
// Modified: wa1tnr, 9 August 2019

#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_SPIFlash.h>


/* flash_ops.cpp
#if defined(__SAMD51__) || defined(NRF52840_XXAA)
  Adafruit_FlashTransport_QSPI flashTransport(PIN_QSPI_SCK, PIN_QSPI_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3);
#else
  #if (SPI_INTERFACES_COUNT == 1)
    Adafruit_FlashTransport_SPI flashTransport(SS, &SPI);
  #else
    Adafruit_FlashTransport_SPI flashTransport(SS1, &SPI1);
  #endif
#endif
*/

// flash_ops.cpp Adafruit_SPIFlash flash(&flashTransport);
// flash_ops.cpp FatFileSystem fatfs;

// File thisFile;

void fl_setup(void) {
}

/* flash_ops.cpp
  if (!flash.begin()) {
    Serial.println("Error");
    while(1);
  }
*/

/* flash_ops.cpp
  Serial.print("Flash chip JEDEC ID: 0x"); Serial.println(flash.getJEDECID(), HEX);
*/

/* flash_ops.cpp
  if (!fatfs.begin(&flash)) {
    Serial.println("Error");
    while(1);
  }
*/

/*
  Serial.println("Mounted filesystem ok");
  Serial.print("  fixed filename:  ");
  Serial.print(FILE_NAME);
  Serial.println("   ... resident on flashROM filesystem\n");
*/

/* flash_ops.cpp
  File dataFile = fatfs.open(FILE_NAME, FILE_READ);
  thisFile = (File) dataFile;
*/
  // Serial.println("READING now omitted - was 'has already run.' 10 Aug 16:42z");
