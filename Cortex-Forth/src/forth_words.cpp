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
extern File thisFile;
#define WRITE_FORTH(a) {thisFile.print((a));}

void forth_words(void) {

//  myFile.print(": hello 5 0 do 43 emit ;\r");

// max ( n1 n2 -- max )


// This is a cpp macro -- the parens can be quite far away
// from the quoted string:

      WRITE_FORTH(     ": max over over - 0< if \
                              swap drop -1 then \
                          if \
                              exit then \
                          swap drop ;\r"   )

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


// min ( n1 n2 -- min )
      WRITE_FORTH(     ": min over over - 0< if \
                              drop exit then \
                          swap drop ;\r"

// testc ( -- )

    ) WRITE_FORTH(     ": testcc -1 512 0 do 1 + dup , loop ;\r"

// >prn ( n -- )
    ) WRITE_FORTH(     ": >prn 32 over over - 0< if \
                              46 emit drop drop exit then \
                          drop 127 1 - over over swap - 0< if \
                              46 emit drop drop exit then \
                          drop emit ;\r"

// delay ( n -- )
    ) WRITE_FORTH(     ": delay drop 1234 0 do 1 drop loop ;\r"

// ecol ( -- ) \ emit a colon
    ) WRITE_FORTH(     ": ecol 58 emit ;\r"

// hadr ( addr -- addr ) print to display
    ) WRITE_FORTH(     ": hadr dup 1 + h. ecol space ;\r")

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
      WRITE_FORTH(     ": rhlist hadr 16 + dup 16 - over over \
                        do \
                            1 + over over \
                            swap - 1 - 0< if \
                                dup rbyte dup 16 - 0< if \
                                    48 emit then \
                                h. 100 delay then \
                        loop \
                        drop ;\r"

// --- all above good 03 SEP 2019

// ralist ( addr -- )

/*
43 emit 43 emit 43 emit 43 emit \
*/

    ) WRITE_FORTH(     ": ralist space space 16 + dup 16 - over over do \
 1 + over over swap - 1 - 0< if dup rbyte >prn 100 delay then loop drop ;\r"


// hlist ( addr -- )
    ) WRITE_FORTH(     ": hlist hadr \
                          16 + dup 16 - over over \
                          do \
                              1 + over over \
                              swap - 1 - 0< if \
                                  dup c@ dup 16 - 0< if \
                                      48 emit then \
                                  h. 100 delay then \
                          loop \
                          drop ;\r"

// alist ( addr -- )
    ) WRITE_FORTH(     ": alist space space 16 + dup 16 - over over \
                          do \
                              1 + over over \
                              swap - 1 - 0< if \
                                  dup c@ >prn \
                                  100 delay then \
                          loop \
                          drop ;\r"

// bottom ( -- addr )
    ) WRITE_FORTH(     ": bottom 536870912 ;\r"

    ) WRITE_FORTH(     ": topbottom bottom 16384 + 1024 - 1024 + 16 - ;\r"

// blist ( addr -- )
    ) WRITE_FORTH(     ": blist cr -999 swap 196604 1148 - min 0 max \
                        dup 1 - 8 0 do dup hlist 16 - alist cr \
                        swap drop loop 1 + swap drop cr ;\r"

// rlist ( addr -- addr + report_size )
    ) WRITE_FORTH(     ": rlist cr -999 swap bottom 195552 + min 0 max \
                        dup 1 - 8 0 do dup rhlist 16 - ralist cr \
                        swap drop loop 1 + swap drop cr ;\r"

// : rlist -999 swap cr bottom 195552 + min 0 max
// dup 1 - 8 0 do
// dup rhlist 16 - ralist cr swap drop
// loop 1 + swap drop cr ;

// at the Ok prompt, type:
    ) WRITE_FORTH(     "wag wag 8 wiggle\r"

// example:
//        bottom 464 + rlist cr 84 blist cr

// rdump was deprecated. 24 Aug

    ) WRITE_FORTH(     ": emits 0 do emit loop space ;\r"

    ) WRITE_FORTH(     ": said fs@ emits space cr space ;\r"

    ) WRITE_FORTH(     ": stuffit 69 68 67 66 65 5 ;\r"


// immediate:


    ) WRITE_FORTH(     "69 68 67 66 65 5 emits cr\r"

    ) WRITE_FORTH(     "2048 allot\r" // 18k address space 03 SEP 2019
    ) WRITE_FORTH(     "variable bend variable buff here buff !\r"
    ) WRITE_FORTH(     "2048 allot here bend ! 1 drop\r"

    ) WRITE_FORTH(     ": svd buff @ 2701 + blist ;\r"  // so adding a 'cr' to the end of the line faked out the parser into not seeing a single character entity as the last entity on the line. ;)

    ) WRITE_FORTH(     ": sve buff @ 4 + cr ;\r"


    ) WRITE_FORTH(     ": goa svd sve 26 0 do 1 + 32 i + over ! loop cr cr svd cr ;\r"



// review:  value address !

    ) WRITE_FORTH(     ": ldelay 1024 0 do 1 delay loop cr ;\r" 

// variable sfi .s empty sfi .s 1144 drop 44 sfi ! .s empty sfi @ .s 44

    // sfi knows if the editor has already been initialized, to
    //    allocate a buffer, or not.
    // sfi was mostly disabled for a bug hunt and hasn't been reinstated fully.

// immediate:

    ) WRITE_FORTH(     "variable sfi 0 sfi ! 1 drop\r"

    ) WRITE_FORTH(     "variable sbc -1 sbc !\r" // sam buffer counter

// key-stored:
    ) WRITE_FORTH(     "variable kst 254 kst ! 1 drop\r"

    ) WRITE_FORTH(     ": bsz 128 ; : bmk bsz 1 - ;\r" // increased from 16 to 128 bytes. ;)

// buffer decrement
    ) WRITE_FORTH(     ": bfd swap 1 - bmk and bmk 2 - over \
                        over swap - 0< if \
                        swap 1 - bmk and \
                        1 - bmk and \
                        1 - bmk and \
                        swap then drop swap ;\r"
// buffer increment
// ( count addr -- count+1 addr )
    ) WRITE_FORTH(     ": bfi \
                        kst @ \
                        200 - 0< if \
                        bfd bfd then \
                        swap 1 + bmk and bmk 2 - over \
                        over swap - 0< if \
                        swap 1 + bmk and \
                        1 + bmk and \
                        1 + bmk and \
                        swap then drop swap ;\r"
// message: here
    ) WRITE_FORTH(     ": mhe 72 emit 101 emit 114 emit 101 emit 58 emit space ;\r"
    ) WRITE_FORTH(     ": bfc 0 ;\r" // any positive int < (bsz - 2) .. or zero


// conditionally initialize the buffer:

/*
    ) WRITE_FORTH(     ": sxa sfi @ invert if mhe\r" // messsage: here
    ) WRITE_FORTH(     "here dup . bsz allot here swap 1 + -1 sfi ! then ;\r"
*/


// unconditionally init:
    ) WRITE_FORTH(     ": sxa mhe here dup . bsz allot here swap 1 + ;\r"
    ) WRITE_FORTH(     ": txb bfi .s cr ;\r"

    ) WRITE_FORTH(     ": lxa -99 sxa ;\r"

// re-initialization protection:
//  ) WRITE_FORTH(     ": sam sfi @ if 1 drop exit then lxa\r"

    ) WRITE_FORTH(     ": sam lxa \
                        bfc swap bfi \
                        over over + begin \
                        sbc @ 1 + sbc !\r" // increment counter
//  value address !

// backspace ASCII 0x08 related processing 8, 199, 254, kst

    // 254 stored in kst to indicate reset
    // 199 stored to indicate backspace key was pressed

    ) WRITE_FORTH(     "254 kst ! 1 drop\r" // reset kst
    ) WRITE_FORTH(     "key dup\r" // ONLY keystroke gained

// send +++ if backspace is pressed:
    ) WRITE_FORTH(     "9 - 0< if 199 kst ! then\r" // ONLY time '9' is useful in compare

// - - - - standard comparison: 254 vs 199
// everyone store:

    // one path chosen from a single pick of one of these two lines
    // which exchanges 'swap c!' for 'swap drop drop' (the swap was factored out)

    ) WRITE_FORTH(     "kst @ negate 253 + 0< if swap c! then \
                        kst @ 200 - 0< if drop drop then\r"

/*

// read-only and  stack balanced, after these two lines (this note needs reverif.)

    OLD OUTPUT LOOP method was to blist everything.

    ) WRITE_FORTH(     "dup blist\r"  // blist ( addr -- addr+incr )
    ) WRITE_FORTH(     "drop\r"

*/


/*

     instead, want a simple emit (to start with):

*/

//  ) WRITE_FORTH(     "dup alist drop\r" // alist will update your address to the new offset if you let it, so dup for a spare copy of tos, alist, then drop the result
// since alist works, it can be picked apart, too:

//  ) WRITE_FORTH(     "if dup c@ >prn 100 delay then loop\r"

//  ) WRITE_FORTH(     "dup c@ 21 min 126 max emit\r"
// since that didn't work, need a stack print to figure out the offset math:
//  ) WRITE_FORTH(     "cr 65 emit 65 emit 65 emit 65 emit 65 emit space space .s space space\r"
// so it's just 'over over +' to get the address wanted:
//  ) WRITE_FORTH(     "over over + c@ cr .s cr 32 max 126 min emit space\r" // may not want that last space

// bsz size of the buffer
// sbc counter for each keystroke given




    ) WRITE_FORTH(     "sbc @ 1 + dup sbc !\r" // sam buffer counter, increment it
    ) WRITE_FORTH(     "sbc @ 125 - 0< if -1 sbc ! then \
                        bsz - 0< invert if\r" // compare sbc to bsz
    ) WRITE_FORTH(         "cr cr cr \
                            67 emit 67 emit 67 emit \
                            cr cr cr\r" // just get their attention
    ) WRITE_FORTH(     "then 1 drop \
                        over over + c@ 32 max 126 min emit\r" // keyboard echo
    ) WRITE_FORTH(     "bfi over over + 1 drop \
                        again ;\r"
    )
}

// \ sam is the name of the built-in editor.

// - - - exercise blist
//   WRITE_FORTH("variable myvar 439041101 myvar c! myvar 32 - blist\r");
//                                         1a2b3c4d
// EOF
