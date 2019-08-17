// flash_ops.cpp  wa1tnr
// Sat Aug 17 23:06:52 UTC 2019

/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include "SdFat.h"
#include "Adafruit_SPIFlash.h"

#undef WANT_MKDIR_FORTH
#define WANT_MKDIR_FORTH

File thisFile;
#include "../common.h"

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

// file system object from SdFat
FatFileSystem fatfs;

File myFile;

#define WORKING_DIR "/forth"

void mkdir_forth(void) {
  if (!fatfs.exists(WORKING_DIR)) {
    Serial.print(WORKING_DIR);
    Serial.println(" directory not found, creating...");
    if (!fatfs.mkdir(WORKING_DIR)) {
      Serial.print("Error, failed to create ");
      Serial.print(WORKING_DIR);
      Serial.println(" directory!");
      Serial.println("Entering an endless loop (as a trap) after a delay of 4 seconds.");
      delay(4000);
      while(1);
    }
    Serial.print("Created ");
    Serial.print(WORKING_DIR);
    Serial.println(" directory!");
  } else {
      Serial.print(WORKING_DIR);
      Serial.println("  directory was previously created.  No worries.  Continuing.. ");
  }
}

// modeled on:

/*
 75   if (!fatfs.exists("/test")) {
 76     Serial.println("Test directory not found, creating...");
 77     // Use mkdir to create directory (note you should _not_ have a trailing slash).
 78     if (!fatfs.mkdir("/test")) {
 79       Serial.println("Error, failed to create test directory!");
 80       while(1);
 81     }
 82     Serial.println("Created test directory!");
 83   }

*/
void flash_setup(void) {
  // Open serial communications and wait for port to open:
  Serial.begin(38400);
  while (!Serial) {
    delay(1); // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("\nInitializing Filesystem on external flash...");
  
  // Init external flash
  flash.begin();

  // Init file system on the flash
  fatfs.begin(&flash);

  Serial.println("initialization done.");

  if (!fatfs.remove(FILE_NAME)) {
    Serial.print("Failed to remove "); Serial.println(FILE_NAME);
  }


#ifdef WANT_MKDIR_FORTH
  mkdir_forth(); // tnr kludge
#endif // #ifdef WANT_MKDIR_FORTH

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = fatfs.open(FILE_NAME, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to "); Serial.print(FILE_NAME); Serial.print(" ");

// file contents - - - - - - - - - - - - - - - -

//  myFile.print("\r");

// max ( n1 n2 -- max )
    myFile.print(": max over over - 0< if swap drop -1 then if exit then swap drop ;\r");

// min ( n1 n2 -- min )
    myFile.print(": min over over - 0< if drop exit then swap drop ;\r");

// >prn ( n -- 31<n<128 )
//  myFile.print(": >prn 32 over over - 0< if 46 emit drop then max 127 over over - 0< if 46 emit drop then min ;\r");

// >prn ( n -- )
    myFile.print(": >prn 32 over over - 0< if 46 emit drop drop exit then drop 127 over over swap - 0< if 46 emit drop drop exit then drop emit ;\r");

// alist ( addr -- )
    myFile.print(": alist 16 + dup 16 - over over\r");
    myFile.print("do 1 + over over swap - 0<\r");
 // myFile.print("if dup c@ >prn emit then loop\r");
    myFile.print("if dup c@ >prn then loop\r");
    myFile.print("drop ;\r");

// blist ( addr -- addr + CONST )
    myFile.print(": blist depth 1 - 0< if 0 then\r");

    myFile.print("196608 1148 - min 1 max 48 0 do alist loop cr ;\r");

/*
.. ...... ....D.. w...a.  ...!D........ .Y......y...........@..;...
.s
  196228  Ok

*/

    myFile.print("wag wag 8 wiggle\r");

    myFile.print(": emits 0 do emit loop ;\r");

    myFile.print(": stuffit 69 68 67 66 65 5 ;\r");

// for some reason, stuffit (above) could not follow after this line, without crashing:

    myFile.print("69 68 67 66 65 5 emits cr\r");

    // myFile.print("wlist cr cr words cr\r");

// file contents - - - - - - - - - - - - - - - -

    // close the file:
    myFile.close();
    Serial.println(" has now been done.");
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening "); Serial.println(FILE_NAME);
  }

  // re-open the file for reading:

  myFile = fatfs.open(FILE_NAME);
  // thisFile = (File) myFile; // local tnr kludge

  if (myFile) {
    Serial.print(FILE_NAME); Serial.println(" .. will now be read and printed");
    Serial.println("to the console.  Attention: design has strange line endings!\r\n");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
    Serial.println("\r\n");
    Serial.print(FILE_NAME); Serial.println(" .. is now closed, safely.");

    // re-open the file for reading:

    File dataFile = fatfs.open(FILE_NAME, FILE_READ);
    Serial.print(FILE_NAME);
    Serial.println(" is now re-opened (for reading).");

    thisFile = (File) dataFile;
    thisFile.rewind();
    Serial.println("FILE STAYS OPEN (and rewound) (for a possible fload).");
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening "); Serial.println(FILE_NAME);
  }
}
