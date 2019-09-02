// flash_ops.cpp  wa1tnr
// Wed Aug 21 02:15:00 UTC 2019 0.1.8 good-compiler-aa-bb  shred: abn-515

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
// #define WORKING_DIR "/forth"
// #define VERBIAGE_AA #undef VERBIAGE_AA

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
#ifdef VERBIAGE_AA
    Serial.print("Created ");
    Serial.print(WORKING_DIR);
    Serial.println(" directory!");
#else
    Serial.print(" +dcr ");
#endif // #ifdef VERBIAGE_AA
  } else {
#ifdef VERBIAGE_AA
      Serial.print(WORKING_DIR);
      Serial.println("  directory was previously created.  No worries.  Continuing.. ");
#else
      Serial.print(" +dpc ");
#endif // #ifdef VERBIAGE_AA
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

#ifdef VERBIAGE_AA
  Serial.print("\nInitializing Filesystem on external flash...");
#else
  Serial.print(" ckpt AA ");
#endif // #ifdef VERBIAGE_AA
  
  // Init external flash
  flash.begin();

  // Init file system on the flash
  fatfs.begin(&flash);

#ifdef VERBIAGE_AA
  Serial.println("initialization done.");
#else
  Serial.print(" ckpt BB ");
#endif // #ifdef VERBIAGE_AA

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
#ifdef VERBIAGE_AA
    Serial.print("Writing to "); Serial.print(FILE_NAME); Serial.print(" ");
#else
    Serial.print(" ckpt CC ");
#endif // #ifdef VERBIAGE_AA

// file contents - - - - - - - - - - - - - - - -

//  myFile.print("\r");

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
    myFile.print(": blist cr -999 swap 196604 1148 - min 0 max\r");
//  myFile.print("196608 1148 - min 0 max 1 - 8 0 do\r");
    myFile.print("dup 1 - 8 0 do dup hlist 16 - alist cr\r");
    myFile.print("swap drop loop 1 + swap drop cr ;\r");


// rlist ( addr -- addr + report_size )
    myFile.print(": rlist cr -999 swap bottom 195552 + min 0 max\r");
    myFile.print("dup 1 - 8 0 do dup rhlist 16 - ralist cr\r");
    myFile.print("swap drop loop 1 + swap drop cr ;\r");

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

    myFile.print(": emits 0 do emit loop space ;\r");
    myFile.print(": said fs@ emits space cr space ;\r");
    myFile.print(": stuffit 69 68 67 66 65 5 ;\r");
    myFile.print("69 68 67 66 65 5 emits cr\r");

    myFile.print("variable bend variable buff here buff !\r");
    myFile.print("32 allot here bend ! 1 drop\r");
    myFile.print(": svd buff @ 2701 + blist ;\r"); // so adding a 'cr' to the end of the line faked out the parser into not seeing a single character entity as the last entity on the line. ;)
    myFile.print(": sve buff @ 4 + cr ;\r");
//  myFile.print(": goa svd sve 26 0 do 1 + 32 i + over ! loop cr cr svd cr ;\r");

#define WRITE_FORTH(a) {myFile.print((a));}
    WRITE_FORTH(": goa svd sve 26 0 do 1 + 32 i + over ! loop cr cr svd cr ;\r")


/*
*/

//  myFile.print(" ;\r");

// - - - exercise blist
//  myFile.print("variable myvar 439041101 myvar c! myvar 32 - blist\r");
//                                         1a2b3c4d

    // myFile.print("wlist cr cr words cr\r");

// file contents - - - - - - - - - - - - - - - -

    // close the file:
    myFile.close();
#ifdef VERBIAGE_AA
    Serial.println(" has now been done.");
#else
    Serial.print(" ckpt DD ");
#endif // #ifdef VERBIAGE_AA
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening "); Serial.println(FILE_NAME);
  }

  // re-open the file for reading:

  myFile = fatfs.open(FILE_NAME);
  // thisFile = (File) myFile; // local tnr kludge

  if (myFile) {
#ifdef VERBIAGE_AA
    Serial.print(FILE_NAME); Serial.println(" .. will now be read and printed");
    Serial.println("to the console.  Attention: design has strange line endings!\r\n");
#else
    Serial.print(" ckpt EE ");
#endif // #ifdef VERBIAGE_AA

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
#ifdef VERBIAGE_AA
      Serial.write(myFile.read());
#else
#ifdef FBUFF_PRN // suppress when undefined
      Serial.print("+");
#endif
      int ij = myFile.read(); // do the read anyway - try to break something
#endif // #ifdef VERBIAGE_AA
    }
    // close the file:
    myFile.close();
#ifdef VERBIAGE_AA
    Serial.println("\r\n");
    Serial.print(FILE_NAME); Serial.println(" .. is now closed, safely.");
#else
    Serial.print(" ckpt FF ");
#endif // #ifdef VERBIAGE_AA

    // re-open the file for reading:

    File dataFile = fatfs.open(FILE_NAME, FILE_READ);
#ifdef VERBIAGE_AA
    Serial.print(FILE_NAME);
    Serial.println(" is now re-opened (for reading).");
#else
    Serial.print(" ckpt GG ");
#endif // #ifdef VERBIAGE_AA

    thisFile = (File) dataFile;
    thisFile.rewind();
#ifdef VERBIAGE_AA
    Serial.println("FILE STAYS OPEN (and rewound) (for a possible fload).");
#else
    Serial.print(" ckpt HH ");
#endif // #ifdef VERBIAGE_AA
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening "); Serial.println(FILE_NAME);
  }
}
