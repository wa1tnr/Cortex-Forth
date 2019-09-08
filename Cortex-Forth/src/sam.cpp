// sam.cpp  wa1tnr
// Sun Sep  8 14:47:04 UTC 2019 0.2.0-alpha.1 non-usart-d-08_sep-aa-split-sam-a-  shred: abn-719"
// Tue Sep  8 12:49:22 UTC 2019 0.2.0-alpha.1 non-usart-c-03_sep-aa-  shred: abn-715"
// On branch non-usart-d-08_sep-aa-split-sam-a-
// prev: On branch non-usart-c-03_sep-aa-

/*
  SD card read/write

 */

// #include "SdFat.h"
#include "../common.h"
// extern File thisFile;
// #define WRITE_FORTH(a) {thisFile.print((a));}

void sam_editor(void) {

// 0= max min > prn delay ecol hadr rhlist ralist hlist alist
// bottom topbottom blist rlist emits

// immediate:
      WRITE_FORTH(     "2048 allot\r" // 18k address space 03 SEP 2019
    ) WRITE_FORTH(     "variable bend variable buff here buff !\r"


    ) WRITE_FORTH(     "variable bend variable buff here buff !\r"
    ) WRITE_FORTH(     "2048 allot here bend ! 1 drop\r"

    ) WRITE_FORTH(     ": svd buff @ 2701 + blist ;\r"  // so adding a 'cr' to the end of the line faked out the parser into not seeing a single character entity as the last entity on the line. ;)

    ) WRITE_FORTH(     ": sve buff @ 4 + cr ;\r"


    ) WRITE_FORTH(     ": goa svd sve 26 0 do 1 + 32 i + over ! loop cr cr svd cr ;\r"


// review:  value address !

//  ) WRITE_FORTH(     ": ldelay 1024 0 do 1 delay loop ;\r"

// variable sfi .s empty sfi .s 1144 drop 44 sfi ! .s empty sfi @ .s 44

    // sfi knows if the editor has already been initialized, to
    //    allocate a buffer, or not.
    // sfi was mostly disabled for a bug hunt and hasn't been reinstated fully.

// immediate:

    ) WRITE_FORTH(     "variable sfi 0 sfi ! 1 drop\r"

// NAME CHANGE: sbc to kbi keyboard index
// kbi is NOT maintained by k++ and k-- whatsoever.

    ) WRITE_FORTH(     "variable kbi -1 kbi !\r" // sam buffer counter

// key-stored:
    ) WRITE_FORTH(     "variable kst 254 kst ! 1 drop\r"

    ) WRITE_FORTH(     ": bsz 128 ; : bmk bsz 1 - ;\r" // increased from 16 to 128 bytes. ;)



/*

 The_easiest_way_to_   .s 5571 fs@ .s 95 111 116 95 121 97 119 95 116 115 101 
105 115 97 101 95 101 104 84 19 

104 84 19 s" ss  assi ?                                                                                        
.s empty s" create_these_long_text_strings_ ERROR INPUT > 30chars + .s 5635 cr                                 
drop .s empty s" create_these_long_tesxt_  .s 5667 fs@ .s 95 116 120 115 101 116 95 103 110 111 108 95 101 115 
101 104 116 95 101 116 97 101 114 99 24 


s" _text_strings_  fs@ .s 95 115 103 110 105 114 116 115 95 116 120 101 116 95 14

*/
  // ) WRITE_FORTH(  " The_easiest_way_to_
  ) WRITE_FORTH(  ": wep 32 111 116 32 121 97 119 32 116 115 101 105 115 97 101 32 101 104 84 19 emits ;\r"

 // print .s to the terminal

 // sbl word ( addr -- same_addr )  'say blist'
    ) WRITE_FORTH(     ": sbl dup blist drop ;\r"

 // ) WRITE_FORTH(     ": tellme space space cr 8 0 do 43 emit loop space space 46 emit 115 emit space .s space space\r"

#define TELLME_LONG
#undef TELLME_LONG

#ifdef TELLME_LONG
 // improve tellme to ( n -- ) tell me using a specific char repeated:
    ) WRITE_FORTH(     ": tellme dup space space cr 8 0 do emit dup loop drop drop space space \
  46 emit 115 emit space .s space space\r"

// write kst
    ) WRITE_FORTH(     "107 emit 115 emit 116 emit 58 emit space\r"
    ) WRITE_FORTH(     "kst @ h. space space space\r"

// write kbi
// ##bookmark
    ) WRITE_FORTH(     "107 emit 98 emit 105 emit 58 emit space\r"
    ) WRITE_FORTH(     "kbi @ h. space space space\r"
// s" Stack_Depth:_  fs@ .s 95 58 104 116 112 101 68 95 107 99 97 116 83 13 

    ) WRITE_FORTH(     "58 104 116 112 101 68 20 107 99 97 116 83 12\r"
    ) WRITE_FORTH(     "emits space depth . cr ;\r"
#else
    ) WRITE_FORTH(     ": tellme drop ;\r"
#endif


    ) WRITE_FORTH(     ": tstbb 144 kst ! 155 kbi ! 74 tellme ;\r"










// buffer and keyboard conflated in naming due to small namespace for word names ;)
// so generally a 'b' word and a 'k' word refer to similar things.

// buffer decrement
// -99 1 1500  k-- .s -99 0 1500  .. with zero a forbidden location that was reached.
// however, ONLY k++ calls it, so it's a ruse.

// BUGFIX TODO_: needs drop swap but conditionally.
// seems okay otherwise.

// ( count addr -- count-1 addr) // maybe maybe
// -- 
    ) WRITE_FORTH(     ": k-- swap 1 - bmk and bmk 2 - over \
                        over swap - 0< if \
                        swap 1 - bmk and \
                        1 - bmk and \
                        1 - bmk and \
                        swap then drop\r"
    ) WRITE_FORTH(     "dup 0= if 1 - bmk and bmk 2 - then swap ;\r"
// buffer increment
// ( count addr -- count+1 addr )

// k++ basically increments 1 under TOS by one, in a modulo 128 counting arrangement.
// it also skips forbidden locations 0, 126 and 127.
    ) WRITE_FORTH(     ": k++ \
                        kst @ \
                        199 - 0= if \
                        k-- k-- then \
                        swap 1 + bmk and bmk 2 - over \
                        over swap - 0< if \
                        swap 1 + bmk and \
                        1 + bmk and \
                        1 + bmk and \
                        swap then drop swap ;\r"

// wrappers p-- and p++
// : p-- space space k-- space space .s space cr ;
    ) WRITE_FORTH(     ": p-- space space k-- space space .s space cr ;\r"

// : p++ space space k++ space space .s space cr ;
    ) WRITE_FORTH(     ": p++ space space k++ space space .s space cr ;\r"

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

    ) WRITE_FORTH(     ": lxa -99 sxa ;\r"


// sam library:

// ctrl! ( n -- n ) // store any control char as ascii 31, into kst
// wanted: ascii 15 (Control O)
    ) WRITE_FORTH(     ": ctrl! dup 31 - 0< if 31 kst ! then 67 tellme ;\r"

// 29 -- 29 29 -- 29 29 31 -- 29 -2 -- 29 BOOL 

// bksp! ( n -- n ) // store CTRL H (ASCII 0x08) as ascii 199, into kst
    ) WRITE_FORTH(     ": bksp! dup 8 - 0= if 199 kst ! then 80 tellme ;\r"

    ) WRITE_FORTH(     ": escp! dup 27 - 0= if 227 kst ! then 84 tellme ;\r"


    ) WRITE_FORTH(     ": sesc 95 67 83 69 95 83 84 73 95 89 69 72 12 emits ;\r"

// hes handle ESC ascii 27
    ) WRITE_FORTH(     ": hes kst @ 227 - 0= if sesc drop quit then ;\r"

// hco handle control o (<32 is any control key so using 31 here)
    ) WRITE_FORTH(     ": hco kst @ 31 - 0= if kbi @ 1 - kbi !\r"
    ) WRITE_FORTH(     "99 tellme 99 tellme drop drop swap 1 - swap\r"
    ) WRITE_FORTH(     "1 1\r"
    ) WRITE_FORTH(     "10 tellme 100 tellme drop drop sbl then 109 tellme ;\r"

// hbk handle backspace
    ) WRITE_FORTH(     ": hbk kst @\r"
    ) WRITE_FORTH(     "199 - 0= if 1 drop then ;\r"

// re-initialization protection:
//  ) WRITE_FORTH(     ": sam sfi @ if 1 drop exit then lxa\r"

    ) WRITE_FORTH(     ": sam lxa \
                        bfc swap k++ \
                        over over + begin \
                        kbi @ 1 + kbi !\r" // increment counter

// with each iteration through the begin ..
//  value address !

// backspace ASCII 0x08 related processing 8, 199, 254, kst

    // 254 stored in kst to indicate reset
    // 199 stored to indicate backspace key was pressed

    ) WRITE_FORTH(     "254 kst ! 1 drop\r" // reset kst
    ) WRITE_FORTH(     "key\r" // ONLY keystroke gained

// "right: 114 105 103 104 116 5"
//  ) WRITE_FORTH(     "114 emit 105 emit 103 emit 104 emit 116 emit space\r"

// "after: 97 emit 102 emit 116 emit 101 emit 114 emit"
//  ) WRITE_FORTH(     "97 emit 102 emit 116 emit 101 emit 114 emit space cr\r"


    ) WRITE_FORTH(     "ctrl! bksp!\r" // reversing these masks a backspace into just a generic control keyset press ;)

    ) WRITE_FORTH(     "escp!\r"
    ) WRITE_FORTH(     "hes\r"

    ) WRITE_FORTH(     "hbk\r"
// send +++ if backspace is pressed:
//  ) WRITE_FORTH(     "9 - 0< if 199 kst ! then\r" // ONLY time '9' is useful in compare

//  ) WRITE_FORTH(     "bksp!\r"

// new line 16:12z 03 SEP:
// test for ascii 31 or lower (global):
// if it's 32:
//  ( n -- n BOOL )
//  ) WRITE_FORTH(     "dup 31 - 0< if  31 kst ! then\r"
//  ) WRITE_FORTH(     "ctrl!\r"  //  dup 31 - 0< if  31 kst ! then ;\r"

// - - - - standard comparison: 254 vs 199
// everyone store:

    // one path chosen from a single pick of one of these two lines
    // which exchanges 'swap c!' for 'swap drop drop' (the swap was factored out)

    ) WRITE_FORTH(     "kst @ 254 - 0= if swap c! then \
                        kst @ 199 - 0= if drop drop then\r"

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
// kbi counter for each keystroke given




//  ) WRITE_FORTH(     "kbi @ 1 + kbi !\r" // sam buffer counter, increment it

// nothing -- addr -- byte -- byte 1 -- SUM -- SUM SUM -- SUM SUM addr -- SUM

/*
    ) WRITE_FORTH(     "cr 8 0 do 44 emit loop tellme\r"

    ) WRITE_FORTH(     "kbi @ 2 - 0< if\r"
    ) WRITE_FORTH(     "kst @ 28 1 + - 0< if\r"
    ) WRITE_FORTH(     "kst @ 28 - 0< if 1 drop then\r"
    ) WRITE_FORTH(     "swap dup blist drop swap then then\r"
*/

//  ) WRITE_FORTH(     "over blist drop\r"

    ) WRITE_FORTH(     "77 tellme\r"

/*
    ) WRITE_FORTH(     "kbi @ 125 - 0< if -1 kbi ! then \
                        bsz - 0< invert if\r" // compare kbi to bsz

    ) WRITE_FORTH(         "cr cr cr \
                            67 emit 67 emit 67 emit \
                            cr cr cr\r" // just get their attention

    ) WRITE_FORTH(     "then 1 drop \
                        over over +\r"

*/
// sbl is a blist that is wrapped in dup and drop
//  ) WRITE_FORTH(     "78 tellme sbl\r"



/*
    ) WRITE_FORTH(     "c@ 32 max 126 min emit\r" // keyboard echo
*/

    ) WRITE_FORTH(     "hco\r"
    ) WRITE_FORTH(     "k++ over over + 1 drop \
                        again ;\r"
    )
}

// \ sam is the name of the built-in editor.

// EOF
