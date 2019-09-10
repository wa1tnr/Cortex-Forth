// forth_words.cpp  wa1tnr
// Tue Sep  3 12:49:22 UTC 2019 0.2.0-alpha.1 non-usart-c-03_sep-aa-  shred: abn-715"
// On branch non-usart-c-03_sep-aa-

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

#include "SdFat.h"
#include "../common.h"
// extern File thisFile;
// #define WRITELN_FORTH(a) {thisFile.print((a));}

void forth_words(void) {

//  myFile.print(": hello 5 0 do 43 emit ;");

// max ( n1 n2 -- max )


// This is a cpp macro -- the parens can be quite far away
// from the quoted string:

// 0= ( n1 -- BOOL ) // not thoroughly tested, but looked good.
//    WRITELN_FORTH(     ": 0= dup abs negate 0< invert swap drop ;" 
      WRITELN_FORTH(     ": 0= dup abs negate 0< invert swap drop ;"
    ) WRITELN_FORTH(     ": max over over - 0< if swap drop -1 then if exit then swap drop ;"
    ) WRITELN_FORTH(     ": min over over - 0< if drop exit then swap drop ;"
    ) WRITELN_FORTH(     ": testcc -1 512 0 do 1 + dup , loop ;" )

// min ( n1 n2 -- min )

//    WRITELN_FORTH(     ": max over over - 0< if swap drop -1 then if exit then swap drop ;"   )

// The quoted string can be extended with a backslash.

// IT IS POSSIBLE to make no apparent mistake, and
// even so, the compiler _FLPARSE() won't quite accept
// the definition.

// The system will kick out a tilde ('~') to suggest a
// problem (as it uses boilerplate from the 'quit' word,
// which uses a '?' in the same location).

// 1215   DATA(208, '~') // was '?' in the original

// The usual solution is to slighly reorganize the code,
// or add a trailing '\r' (the entire system is oddly
// sensitive to line endings and small groups of characters
// on a line, such as a standalone bang ('!').

// The presence of excess '1 drop' phrases is due to
// trying to fool the parser into accepting the code
// as written.

// As this is player-piano (massive playback of a stored
// file, through the Forth parsing system) it seems a
// bit more sensitive than the equivalent, live
// interactive method (through the keyboard and terminal
// interfaces).

// _FLPARSE() could use a good going-through; its
// control structure/program flow was determined
// experimentally (and, somewhat ignorantly ;)


// testc ( -- )

//    WRITELN_FORTH(     ": testcc -1 512 0 do 1 + dup , loop ;"

// >prn ( n -- )
      WRITELN_FORTH(     ": >prn 32 over over - "
    ) WRITELN_FORTH(     "  0< if "
    ) WRITELN_FORTH(     "      46 emit drop drop exit "
    ) WRITELN_FORTH(     "  then "
    ) WRITELN_FORTH(     "  drop 127 1 - over over swap - "
    ) WRITELN_FORTH(     "  0< if "
    ) WRITELN_FORTH(     "      46 emit drop drop exit "
    ) WRITELN_FORTH(     "  then "
    ) WRITELN_FORTH(     "  drop emit ;"

// delay ( n -- )
    ) WRITELN_FORTH(     ": delay drop 1234 0 do 1 drop loop ;"


// ecol ( -- ) \ emit a colon
    ) WRITELN_FORTH(     ": ecol 58 emit ;"

// hadr ( addr -- addr ) print to display
    ) WRITELN_FORTH(     ": hadr dup 1 + h. ecol space ;")

/*
// Can pack memory efficiently using the comma word:

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
      WRITELN_FORTH(     ": rhlist hadr 16 + dup 16 - over over "
    ) WRITELN_FORTH(     "  do "
    ) WRITELN_FORTH(     "      1 + over over swap - 1 - "
    ) WRITELN_FORTH(     "      0< if "
    ) WRITELN_FORTH(     "          dup rbyte dup 16 - "
    ) WRITELN_FORTH(     "          0< if "
    ) WRITELN_FORTH(     "              48 emit "
    ) WRITELN_FORTH(     "          then "
    ) WRITELN_FORTH(     "          h. 100 delay "
    ) WRITELN_FORTH(     "      then "
    ) WRITELN_FORTH(     "  loop "
    ) WRITELN_FORTH(     "  drop ;"

// --- all above good 03 SEP 2019

// ralist ( addr -- )


    ) WRITELN_FORTH(     ": ralist space space 16 + dup 16 - over over do 1 + over over swap - 1 - 0< if dup rbyte >prn 100 delay then loop drop ;"


// hlist ( addr -- )
    ) WRITELN_FORTH(     ": hlist hadr 16 + dup 16 - over over do 1 + over over swap - 1 - 0< if dup c@ dup 16 - 0< if 48 emit then h. 100 delay then loop drop ;"

// alist ( addr -- )
    ) WRITELN_FORTH(     ": alist space space 16 + dup 16 - over over do 1 + over over swap - 1 - 0< if dup c@ >prn 100 delay then loop drop ;"

// bottom ( -- addr )
    ) WRITELN_FORTH(     ": bottom 536870912 ;"

    ) WRITELN_FORTH(     ": topbottom bottom 16384 + 1024 - 1024 + 16 - ;"

// blist ( addr -- )
    ) WRITELN_FORTH(     ": blist cr -999 swap 196604 1148 - min 0 max dup 1 - 8 0 do dup hlist 16 - alist cr swap drop loop 1 + swap drop cr ;"

// rlist ( addr -- addr + report_size )
    ) WRITELN_FORTH(     ": rlist cr -999 swap bottom 195552 + min 0 max dup 1 - 8 0 do dup rhlist 16 - ralist cr swap drop loop 1 + swap drop cr ;"

// : rlist -999 swap cr bottom 195552 + min 0 max
// dup 1 - 8 0 do
// dup rhlist 16 - ralist cr swap drop
// loop 1 + swap drop cr ;

// at the Ok prompt, type:
//  ) WRITELN_FORTH(     "wag wag 8 wiggle"

// example:
//        bottom 464 + rlist cr 84 blist cr

// rdump was deprecated. 24 Aug
    ) WRITELN_FORTH(     ": emits 0 do emit loop space ;"

    ) WRITELN_FORTH(     ": said fs@ emits space cr space ;"

// KLUDGE - immediate lines conflict with the parser as it is now (09 SEP 2019)

// Solution (temporary) is to write one long line 
// of code, that has the immediate code somewhere
// within it (perhaps followed by non-immediate code
// on that same line, as in the example, below).
// for immediate lines of code, on the previous line,
// do not WRITELN_FORTH.  Also, add an extra space:

    ) WRITE_FORTH(     ": stuffit 69 68 67 66 65 5 ; "

// immediate:
// the immediate line is not a WRITELN_FORTH line:

    ) WRITE_FORTH(     "69 68 67 66 65 5 emits cr "

// the immediate line is followed by a standard WRITELN_FORTH line:

//  ) WRITELN_FORTH(     ": ldelay 1024 0 do 1 delay loop ;"

// review:  value address !

// end, pickup with sam.cpp from here, forward.

#ifdef OMITTED_CODE
#endif // OMITTED_CODE
)
}

// - - - exercise blist
//   WRITELN_FORTH("variable myvar 439041101 myvar c! myvar 32 - blist");
//                                         1a2b3c4d
// EOF
