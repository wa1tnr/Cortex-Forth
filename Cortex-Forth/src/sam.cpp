// sam.cpp  wa1tnr
// Tue Sep 10 13:52:18 UTC 2019
// On branch non-usart--testing-a

// Tue Sep 10 00:25:56 UTC 2019 0.2.0-alpha.2 non-usart--testing-a  shred: abn-737

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
// #define WRITELN_FORTH(a) {thisFile.print((a));}

void sam_editor(void) {

// 0= max min > prn delay ecol hadr rhlist ralist hlist alist
// bottom topbottom blist rlist emits

// immediate:


      WRITE_VERT_WSPACE(  "  "
    ) WRITE_VERT_WSPACE(  "  "
    )   WRITELN_FORTH(     "2048 allot " // 18k address space 03 SEP 2019
    )   WRITELN_FORTH(     "variable bend variable buff here buff ! "
    )   WRITELN_FORTH(     "variable bend variable buff here buff ! "
    )   WRITELN_FORTH(     "2048 allot here bend ! 1 drop "
    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     ": svd buff @ 2701 + blist ;"  // so adding a 'cr' to the end of the line faked out the parser into not seeing a single character entity as the last entity on the line. ;)
    ) WRITELN_FORTH(     ": sve buff @ 4 + cr ;"

    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     ": goa svd sve 26 0 do 1 + 32 i + over ! loop cr cr svd cr ;"
    ) WRITE_VERT_WSPACE(  "  " )

// review:  value address !

//  ) WRITELN_FORTH(     ": ldelay 1024 0 do 1 delay loop ;"

// variable sfi .s empty sfi .s 1144 drop 44 sfi ! .s empty sfi @ .s 44

    // sfi knows if the editor has already been initialized, to
    //    allocate a buffer, or not.
    // sfi was mostly disabled for a bug hunt and hasn't been reinstated fully.

// immediate:

        WRITELN_FORTH(     "variable sfi 0 sfi ! 1 drop"

// NAME CHANGE: sbc to kbi keyboard index
// kbi is NOT maintained by k++ and k-- whatsoever.

      ) WRITELN_FORTH(     "variable kbi -1 kbi !" // sam buffer counter

// key-stored:
      ) WRITELN_FORTH(     "variable kst 254 kst ! 1 drop"

    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     ": bsz 128 ; : bmk bsz 1 - ;" ) // increased from 16 to 128 bytes. ;)




// bsz ( -- size )
// bmk ( -- mask )



/*

 The_easiest_way_to_   .s 5571 fs@ .s 95 111 116 95 121 97 119 95 116 115 101 
105 115 97 101 95 101 104 84 19 

104 84 19 s" ss  assi ?                                                                                        
.s empty s" create_these_long_text_strings_ ERROR INPUT > 30chars + .s 5635 cr                                 
drop .s empty s" create_these_long_tesxt_  .s 5667 fs@ .s 95 116 120 115 101 116 95 103 110 111 108 95 101 115 
101 104 116 95 101 116 97 101 114 99 24 


s" _text_strings_  fs@ .s 95 115 103 110 105 114 116 115 95 116 120 101 116 95 14

*/
//  ) WRITELN_FORTH(  " The_easiest_way_to_
      WRITELN_FORTH(  ": wep 32 111 116 32 121"
    ) WRITELN_FORTH(  "      97 119 32 116 115"
    ) WRITELN_FORTH(  "      101 105 115 97 101"
    ) WRITELN_FORTH(  "      32 101 104 84 19     emits ;" )

      WRITE_VERT_WSPACE(  "  " )

 // print .s to the terminal

 // sbl word ( addr -- same_addr )  'say blist'
      WRITELN_FORTH(     ": sbl dup blist drop ;" )

 // ) WRITELN_FORTH(     ": tellme space space cr 8 0 do 43 emit loop space space 46 emit 115 emit space .s space space"

#define TELLME_LONG
#undef TELLME_LONG

#ifdef TELLME_LONG
 // improve tellme to ( n -- ) tell me using a specific char repeated:
      WRITELN_FORTH(     ": tellme dup space space cr"
    ) WRITELN_FORTH(     "  8 0 do emit dup loop"
    ) WRITELN_FORTH(     "  drop drop space space"
    ) WRITELN_FORTH(     "  46 emit 115 emit"
    ) WRITELN_FORTH(     "  space .s space space"

// write kst
    ) WRITELN_FORTH(     "  107 emit 115 emit 116 emit 58 emit space"
    ) WRITELN_FORTH(     "  kst @ h. space space space"

// write kbi

    ) WRITELN_FORTH(     "  107 emit 98 emit 105 emit 58 emit space"
    ) WRITELN_FORTH(     "  kbi @ h. space space space"
// s" Stack_Depth:_  fs@ .s 95 58 104 116 112 101 68 95 107 99 97 116 83 13 

    ) WRITELN_FORTH(     "  58 104 116 112 101"
    ) WRITELN_FORTH(     "  68 20 107 99 97"
    ) WRITELN_FORTH(     "  116 83 12"
    ) WRITELN_FORTH(     "  emits space"
    ) WRITELN_FORTH(     "  depth . cr ;" )
#else
      WRITELN_FORTH(     ": tellme drop ;" )
#endif


//  ) WRITELN_FORTH(     ": tstbb 144 kst ! 155 kbi ! 74 tellme ;"


// buffer and keyboard conflated in naming due to small namespace for word names ;)
// so generally a 'b' word and a 'k' word refer to similar things.

// buffer decrement
// -99 1 1500  k-- .s -99 0 1500  .. with zero a forbidden location that was reached.
// however, ONLY k++ calls it, so it's a ruse.

// BUGFIX TODO_: needs drop swap but conditionally.
// seems okay otherwise.

// k-- ( count addr -- count-1 addr)
// -- 

// ###bookmark
//  ) WRITELN_FORTH(     "

      WRITELN_FORTH(     ": sxt 1 - bmk and ; \\ ( n -- masked )"
    ) WRITELN_FORTH(     ": sxo bmk and bmk 2 - ;"
    ) WRITELN_FORTH(     ": sxk sxo over over ;" )

// sxF ( n1 n2 -- n2 a b c )
      WRITELN_FORTH(     ": sxf swap 1 + sxk swap - ;"

    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     ": k-- swap 1 - sxk swap -"
    ) WRITELN_FORTH(     "  0< if"
    ) WRITELN_FORTH(     "      swap sxt sxt sxt swap"
    ) WRITELN_FORTH(     "  then"
    ) WRITELN_FORTH(     "  drop dup"
    ) WRITELN_FORTH(     "  0= if"
    ) WRITELN_FORTH(     "      1 - sxo"
    ) WRITELN_FORTH(     "  then"
    ) WRITELN_FORTH(     "  swap ;" )

// buffer increment
// ( count addr -- count+1 addr )

// k++ basically increments 1 under TOS by one, in a modulo 128 counting arrangement.
// it also skips forbidden locations 0, 126 and 127.

      WRITE_VERT_WSPACE(  "  "
// sxc ( a -- b )
    ) WRITELN_FORTH(     ": sxc 1 + bmk and ;"

// sxd ( a -- b )
    ) WRITELN_FORTH(     ": sxd 3 0 do sxc loop ;"

// sxe ( n -- )
    ) WRITELN_FORTH(     ": sxe 0< if swap sxd swap then ;" )

// sxg ( n -- )
//  ) WRITELN_FORTH(     ": sxg 0= if k-- k-- then ;"
// k-- ( count addr -- count-1 addr)
//  ) WRITELN_FORTH(     ": sxh sxg sxf sxe ;"

      WRITE_VERT_WSPACE(  "  " )

      WRITELN_FORTH(     ": sxh"
    ) WRITELN_FORTH(     "  0= if"
    ) WRITELN_FORTH(     "      k-- k--"
    ) WRITELN_FORTH(     "  then"
    ) WRITELN_FORTH(     "  sxf sxe ;"

    ) WRITE_VERT_WSPACE(  "  "

    ) WRITELN_FORTH(     ": k++ kst @ 199 - sxh drop swap ;" )

// ###bookmark 10 SEP

// wrappers p-- and p++
// : p-- space space k-- space space .s space cr ;
      WRITELN_FORTH(     ": p-- space space k-- space space .s space cr ;"

// : p++ space space k++ space space .s space cr ;
    ) WRITELN_FORTH(     ": p++ space space k++ space space .s space cr ;"

// message: here
    ) WRITELN_FORTH(     ": mhe"
    ) WRITELN_FORTH(     "   72 emit"
    ) WRITELN_FORTH(     "  101 emit"
    ) WRITELN_FORTH(     "  114 emit"
    ) WRITELN_FORTH(     "  101 emit"
    ) WRITELN_FORTH(     "   58 emit space ;" )

// mhe ( -- )
      WRITELN_FORTH(     ": bfc 0 ;" // any positive int < (bsz - 2) .. or zero


// conditionally initialize the buffer:

/*
    ) WRITELN_FORTH(     ": sxa sfi @ invert if mhe" // messsage: here
    ) WRITELN_FORTH(     "here dup . bsz allot here swap 1 + -1 sfi ! then ;"
*/


// unconditionally init:
// sxa ( -- end_addr beg_addr )
    ) WRITELN_FORTH(     ": sxa mhe here dup . bsz allot here swap 1 + ;"
// -- here -- here here -- here -- here size -- here -- here new_here -- new_here here -- new_here here+1

// lxa ( -- -99 end_addr beg_addr ) // returns three bytes
    ) WRITELN_FORTH(     ": lxa -99 sxa ;"


// sam library:

// ctrl! ( n -- n ) // store any control char as ascii 31, into kst
// wanted: ascii 15 (Control O)
    ) WRITELN_FORTH(     ": ctrl!"
    ) WRITELN_FORTH(     "  dup 31 -"
    ) WRITELN_FORTH(     "  0< if"
    ) WRITELN_FORTH(     "      31 kst !"
    ) WRITELN_FORTH(     "  then"
    ) WRITELN_FORTH(     "  67 tellme ;"

// 29 -- 29 29 -- 29 29 31 -- 29 -2 -- 29 BOOL 

// bksp! ( n -- n ) // store CTRL H (ASCII 0x08) as ascii 199, into kst
    ) WRITELN_FORTH(     ": bksp!"
    ) WRITELN_FORTH(     "  dup 8 -"
    ) WRITELN_FORTH(     "  0= if"
    ) WRITELN_FORTH(     "      199 kst !"
    ) WRITELN_FORTH(     "  then"
    ) WRITELN_FORTH(     "  80 tellme ;"


    ) WRITELN_FORTH(     ": escp!"
    ) WRITELN_FORTH(     "  dup 27 -"
    ) WRITELN_FORTH(     "  0= if"
    ) WRITELN_FORTH(     "      227 kst !"
    ) WRITELN_FORTH(     "  then"
    ) WRITELN_FORTH(     "  84 tellme ;"

    ) WRITELN_FORTH(     ": sesc"
    ) WRITELN_FORTH(     "  95 67 83 69 95"
    ) WRITELN_FORTH(     "  83 84 73 95 89"
    ) WRITELN_FORTH(     "  69 72 12"
    ) WRITELN_FORTH(     "  emits ;"

// hes handle ESC ascii 27
    ) WRITELN_FORTH(     ": hes kst @ 227 -"
    ) WRITELN_FORTH(     "  0= if"
    ) WRITELN_FORTH(     "      sesc drop quit"
    ) WRITELN_FORTH(     "  then ;"

    ) WRITELN_FORTH(     ": hcp drop drop swap 1 - swap ;" )

      WRITE_VERT_WSPACE(  "  "

// hco handle control o (<32 is any control key so using 31 here)
//  ) WRITELN_FORTH(     "    " )
    ) WRITELN_FORTH(       ": hco kst @ 31 -"
    ) WRITELN_FORTH(       "  0= if "
    ) WRITELN_FORTH(       "      kbi @ 1 - kbi ! "
    ) WRITELN_FORTH(       "      hcp sbl "
    ) WRITELN_FORTH(     "  then ;"

    ) WRITE_VERT_WSPACE(  "  "

// hbk handle backspace
    ) WRITELN_FORTH(     ": hbk kst @ "
    ) WRITELN_FORTH(     "199 - 0= if 1 drop then ;"

// re-initialization protection:
//  ) WRITELN_FORTH(     ": sam sfi @ if 1 drop exit then lxa"

    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     ": sxp"
    ) WRITELN_FORTH(     "  kbi @ 1 + kbi !" // increment counter

// with each iteration through the begin ..
//  value address !

// backspace ASCII 0x08 related processing 8, 199, 254, kst

    // 254 stored in kst to indicate reset
    // 199 stored to indicate backspace key was pressed

    ) WRITELN_FORTH(     "  254 kst ! 1 drop" // reset kst
    ) WRITELN_FORTH(     "  key" // ONLY keystroke gained

// "right: 114 105 103 104 116 5"
//  ) WRITELN_FORTH(     "114 emit 105 emit 103 emit 104 emit 116 emit space"

// "after: 97 emit 102 emit 116 emit 101 emit 114 emit"
//  ) WRITELN_FORTH(     "97 emit 102 emit 116 emit 101 emit 114 emit space cr"


    ) WRITELN_FORTH(     "      ctrl! bksp!" // reversing these masks a backspace into just a generic control keyset press ;)

    ) WRITELN_FORTH(     "      escp!"
    ) WRITELN_FORTH(     "      hes"

    ) WRITELN_FORTH(     "      hbk"
// send +++ if backspace is pressed:
//  ) WRITELN_FORTH(     "9 - 0< if 199 kst ! then" // ONLY time '9' is useful in compare

//  ) WRITELN_FORTH(     "bksp!"

// new line 16:12z 03 SEP:
// test for ascii 31 or lower (global):
// if it's 32:
//  ( n -- n BOOL )
//  ) WRITELN_FORTH(     "dup 31 - 0< if  31 kst ! then"
//  ) WRITELN_FORTH(     "ctrl!"  //  dup 31 - 0< if  31 kst ! then ;"

// - - - - standard comparison: 254 vs 199
// everyone store:

    // one path chosen from a single pick of one of these two lines
    // which exchanges 'swap c!' for 'swap drop drop' (the swap was factored out)

    ) WRITELN_FORTH(     "    kst @ 254 -"
    ) WRITELN_FORTH(     "    0= if"
    ) WRITELN_FORTH(     "        swap c!"
    ) WRITELN_FORTH(     "    then"
    ) WRITELN_FORTH(     "    kst @ 199 -"
    ) WRITELN_FORTH(     "    0= if"
    ) WRITELN_FORTH(     "        drop drop"
    ) WRITELN_FORTH(     "    then"

/*

// read-only and  stack balanced, after these two lines (this note needs reverif.)

    OLD OUTPUT LOOP method was to blist everything.

    ) WRITELN_FORTH(     "dup blist"  // blist ( addr -- addr+incr )
    ) WRITELN_FORTH(     "drop"

*/


/*

     instead, want a simple emit (to start with):

*/

//  ) WRITELN_FORTH(     "dup alist drop" // alist will update your address to the new offset if you let it, so dup for a spare copy of tos, alist, then drop the result
// since alist works, it can be picked apart, too:

//  ) WRITELN_FORTH(     "if dup c@ >prn 100 delay then loop"

//  ) WRITELN_FORTH(     "dup c@ 21 min 126 max emit"
// since that didn't work, need a stack print to figure out the offset math:
//  ) WRITELN_FORTH(     "cr 65 emit 65 emit 65 emit 65 emit 65 emit space space .s space space"
// so it's just 'over over +' to get the address wanted:
//  ) WRITELN_FORTH(     "over over + c@ cr .s cr 32 max 126 min emit space" // may not want that last space

// bsz size of the buffer
// kbi counter for each keystroke given




//  ) WRITELN_FORTH(     "kbi @ 1 + kbi !" // sam buffer counter, increment it

// nothing -- addr -- byte -- byte 1 -- SUM -- SUM SUM -- SUM SUM addr -- SUM

/*
    ) WRITELN_FORTH(     "cr 8 0 do 44 emit loop tellme"

    ) WRITELN_FORTH(     "kbi @ 2 - 0< if"
    ) WRITELN_FORTH(     "kst @ 28 1 + - 0< if"
    ) WRITELN_FORTH(     "kst @ 28 - 0< if 1 drop then"
    ) WRITELN_FORTH(     "swap dup blist drop swap then then"
*/

//  ) WRITELN_FORTH(     "over blist drop"

    ) WRITELN_FORTH(     "  77 tellme"

/*
    ) WRITELN_FORTH(     "kbi @ 125 - 0< if -1 kbi ! then \
                        bsz - 0< invert if" // compare kbi to bsz

    ) WRITELN_FORTH(         "cr cr cr \
                            67 emit 67 emit 67 emit \
                            cr cr cr" // just get their attention

    ) WRITELN_FORTH(     "then 1 drop \
                        over over +"

*/
// sbl is a blist that is wrapped in dup and drop
//  ) WRITELN_FORTH(     "78 tellme sbl"



/*
    ) WRITELN_FORTH(     "c@ 32 max 126 min emit" // keyboard echo
*/

    ) WRITELN_FORTH(     "  hco"
    ) WRITELN_FORTH(     "  k++ over over +"
    ) WRITELN_FORTH(     "  1 drop ;"
// end, sxp definition



//  ) WRITELN_FORTH(     "k++ over over + 1 drop again ;"

    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     "  \\ comments need two leading spaces"
    ) WRITELN_FORTH(     ": hix drop ; \\ I have commented my code also works"
    ) WRITELN_FORTH(     ": sxb begin sxp again ;" )
      WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     "  \\ The sam text editor"
    ) WRITELN_FORTH(     ": sam lxa bfc swap k++ over over + sxb ;" )

} void foobiabaria(void) {

#ifdef OMIT_SOME_SOURCE
#endif

#define OMITTED_CODE_BB
#undef OMITTED_CODE_BB
      WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     "  \\ Consider this short program:"
    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     ": abca 10 ;"
    ) WRITELN_FORTH(     ": abcb 20 ;"
    ) WRITELN_FORTH(     ": abcc 30 ;"
    ) WRITELN_FORTH(     ": bcd abca . abcb . abcc . abcx . ;"
    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     "  \\ when run, bcd gives:"
    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     "  \\     bcd 30 30 30 30 Ok"
    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     "  \\ If a file has comments at the bottom,"
    ) WRITELN_FORTH(     "  \\ include a valid colon definition"
    ) WRITELN_FORTH(     "  \\ below the last of the comments:"
    ) WRITE_VERT_WSPACE(  "  "
    ) WRITELN_FORTH(     ": guard 1 drop ;"
#ifdef OMITTED_CODE_BB
#endif // OMITTED_CODE
    )
}

// \ sam is the name of the built-in editor.

// EOF
