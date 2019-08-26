// flash_ops.cpp  wa1tnr
// Wed Aug 21 02:15:00 UTC 2019 0.1.8 good-compiler-aa-bb  shred: abn-515

// USART is Serial1 and USB is Serial
#define SERIAL_LOCAL_F Serial1

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
    SERIAL_LOCAL_F.print(WORKING_DIR);
    SERIAL_LOCAL_F.println(" directory not found, creating...");
    if (!fatfs.mkdir(WORKING_DIR)) {
      SERIAL_LOCAL_F.print("Error, failed to create ");
      SERIAL_LOCAL_F.print(WORKING_DIR);
      SERIAL_LOCAL_F.println(" directory!");
      SERIAL_LOCAL_F.println("Entering an endless loop (as a trap) after a delay of 4 seconds.");
      delay(4000);
      while(1);
    }
    SERIAL_LOCAL_F.print("Created ");
    SERIAL_LOCAL_F.print(WORKING_DIR);
    SERIAL_LOCAL_F.println(" directory!");
  } else {
      SERIAL_LOCAL_F.print(WORKING_DIR);
      SERIAL_LOCAL_F.println("  directory was previously created.  No worries.  Continuing.. ");
  }
}

// modeled on:

/*
 75   if (!fatfs.exists("/test")) {
 76     SERIAL_LOCAL_F.println("Test directory not found, creating...");
 77     // Use mkdir to create directory (note you should _not_ have a trailing slash).
 78     if (!fatfs.mkdir("/test")) {
 79       SERIAL_LOCAL_F.println("Error, failed to create test directory!");
 80       while(1);
 81     }
 82     SERIAL_LOCAL_F.println("Created test directory!");
 83   }

*/
void flash_setup(void) {
  // Open serial communications and wait for port to open:
  SERIAL_LOCAL_F.begin(38400);
  while (!SERIAL_LOCAL_F) {
    delay(1); // wait for serial port to connect. Needed for native USB port only
  }

  SERIAL_LOCAL_F.print("\nInitializing Filesystem on external flash...");
  
  // Init external flash
  flash.begin();

  // Init file system on the flash
  fatfs.begin(&flash);

  SERIAL_LOCAL_F.println("initialization done.");

  if (!fatfs.remove(FILE_NAME)) {
    SERIAL_LOCAL_F.print("Failed to remove "); SERIAL_LOCAL_F.println(FILE_NAME);
  }


#ifdef WANT_MKDIR_FORTH
  mkdir_forth(); // tnr kludge
#endif // #ifdef WANT_MKDIR_FORTH

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = fatfs.open(FILE_NAME, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    SERIAL_LOCAL_F.print("Writing to "); SERIAL_LOCAL_F.print(FILE_NAME); SERIAL_LOCAL_F.print(" ");

// file contents - - - - - - - - - - - - - - - -

//  myFile.print("\r");

// ASCII 13 is preferred over CR for 9term.

// crd ( - ) // emit 0x0d
    myFile.print(": crd 13 emit ;\r");

// max ( n1 n2 -- max )
    myFile.print(": max over over - 0< if swap drop -1 then if exit then swap drop ;\r");

// min ( n1 n2 -- min )
    myFile.print(": min over over - 0< if drop exit then swap drop ;\r");

// testc ( -- )

    myFile.print(": testcc -1 512 0 do 1 + dup , loop ;\r");

// >prn ( n -- )
    myFile.print(": >prn 32 over over - 0< if 46 emit drop drop exit then drop 127 1 - over over swap - 0< if 46 emit drop drop exit then drop emit ;\r");

// delay ( n -- )
    myFile.print(": delay drop 1234 0 do 1 drop loop ;\r");

// ecol ( -- ) \ emit a colon
    myFile.print(": ecol 58 emit ;\r");

// hadr ( addr -- addr ) print to display
    myFile.print(": hadr dup 1 + h. ecol space ;\r");
/*

Can pack memory efficiently using the comma word:

\ 0abbccdd    1a2b3c4d

180079837 , 439041101 , 3008 16 - blist

BB0 05 00 00 00 19 00 00 00 DD CC BB 0A 4D 3C 2B 1A ............M<+.
BC0 00 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00 ................
BD0 04 00 00 00 05 00 00 00 06 00 00 00 07 00 00 00 ................
BE0 08 00 00 00 09 00 00 00 0A 00 00 00 0B 00 00 00 ................
BF0 0C 00 00 00 0D 00 00 00 0E 00 00 00 0F 00 00 00 ................
C00 10 00 00 00 11 00 00 00 12 00 00 00 13 00 00 00 ................
C10 14 00 00 00 15 00 00 00 16 00 00 00 17 00 00 00 ................
C20 18 00 00 00 19 00 00 00 1A 00 00 00 1B 00 00 00 ................

*/

// rhlist ( addr -- )
    myFile.print(": rhlist hadr 16 + dup 16 - over over\r");
    myFile.print("do 1 + over over swap - 1 - 0<\r");
    myFile.print("if dup rbyte dup 16 - 0< if 48 emit then h. 100 delay then loop\r");
    myFile.print("drop ;\r");

// ralist ( addr -- )
    myFile.print(": ralist space space 16 + dup 16 - over over\r");
    myFile.print("do 1 + over over swap - 1 - 0<\r");
    myFile.print("if dup rbyte >prn 100 delay then loop\r");
    myFile.print("drop ;\r");

// hlist ( addr -- )
    myFile.print(": hlist hadr 16 + dup 16 - over over\r");
    myFile.print("do 1 + over over swap - 1 - 0<\r");
    myFile.print("if dup c@ dup 16 - 0< if 48 emit then h. 100 delay then loop\r");
//  myFile.print("if dup rbyte dup 16 - 0< if 48 emit then h. 100 delay then loop\r");
    myFile.print("drop ;\r");

// alist ( addr -- )
    myFile.print(": alist space space 16 + dup 16 - over over\r");
    myFile.print("do 1 + over over swap - 1 - 0<\r");
    myFile.print("if dup c@ >prn 100 delay then loop\r");
//  myFile.print("if dup rbyte >prn 100 delay then loop\r");
    myFile.print("drop ;\r");

// bottom ( -- addr )
    myFile.print(": bottom 536870912 ;\r");
    myFile.print(": topbottom bottom 16384 + 1024 - 1024 + 16 - ;\r");

// blist ( addr -- )
    myFile.print(": blist crd -999 swap 196604 1148 - min 0 max\r");
//  myFile.print("196608 1148 - min 0 max 1 - 8 0 do\r");
    myFile.print("dup 1 - 8 0 do dup hlist 16 - alist crd\r");
    myFile.print("swap drop loop 1 + swap drop crd ;\r");


// rlist ( addr -- addr + report_size )
    myFile.print(": rlist crd -999 swap bottom 195552 + min 0 max\r");
    myFile.print("dup 1 - 8 0 do dup rhlist 16 - ralist crd\r");
    myFile.print("swap drop loop 1 + swap drop crd ;\r");

/*

: rlist -999 swap cr bottom 195552 + min 0 max
dup 1 - 8 0 do
dup rhlist 16 - ralist cr swap drop
loop 1 + swap drop cr ;

*/

// at the Ok prompt, type:
    myFile.print("wag wag 8 wiggle\r");

// canonical for 24 August:
// rlist and blist

// example:
//        bottom 464 + rlist cr 84 blist cr

// rdump was deprecated. 24 Aug

    myFile.print(": emits 0 do emit loop ;\r");
    myFile.print(": stuffit 69 68 67 66 65 5 ;\r");
    myFile.print("69 68 67 66 65 5 emits crd\r");

// - - - exercise blist
//  myFile.print("variable myvar 439041101 myvar c! myvar 32 - blist\r");
//                                         1a2b3c4d

    // myFile.print("wlist cr cr words cr\r");

// file contents - - - - - - - - - - - - - - - -

    // close the file:
    myFile.close();
    SERIAL_LOCAL_F.println(" has now been done.");
  } else {
    // if the file didn't open, print an error:
    SERIAL_LOCAL_F.print("error opening "); SERIAL_LOCAL_F.println(FILE_NAME);
  }

  // re-open the file for reading:

  myFile = fatfs.open(FILE_NAME);
  // thisFile = (File) myFile; // local tnr kludge

  if (myFile) {
    SERIAL_LOCAL_F.print(FILE_NAME); SERIAL_LOCAL_F.println(" .. will now be read and printed");
    SERIAL_LOCAL_F.println("to the console.  Attention: design has strange line endings!\r\n");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      SERIAL_LOCAL_F.write(myFile.read());
    }
    // close the file:
    myFile.close();
    SERIAL_LOCAL_F.println("\r\n");
    SERIAL_LOCAL_F.print(FILE_NAME); SERIAL_LOCAL_F.println(" .. is now closed, safely.");

    // re-open the file for reading:

    File dataFile = fatfs.open(FILE_NAME, FILE_READ);
    SERIAL_LOCAL_F.print(FILE_NAME);
    SERIAL_LOCAL_F.println(" is now re-opened (for reading).");

    thisFile = (File) dataFile;
    thisFile.rewind();
    SERIAL_LOCAL_F.println("FILE STAYS OPEN (and rewound) (for a possible fload).");
  } else {
    // if the file didn't open, print an error:
    SERIAL_LOCAL_F.print("error opening "); SERIAL_LOCAL_F.println(FILE_NAME);
  }
}
