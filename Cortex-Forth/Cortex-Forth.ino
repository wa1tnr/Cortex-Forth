// Sun Aug 25 03:10:50 UTC 2019 0.1.9 good-compiler-aa-ff-aa_exp  shred: abn-591

#define SERIAL_LOCAL_C Serial1

// old:
// Sat Aug 24 22:24:57 UTC 2019 0.1.9 good-compiler-aa-ff-aa_exp  shred: abn-579


// - - - -   snippet   - - - -

// next (  - ) increment address and emit hex of byte stored at 1+ current address
// : next 1 + dup rbyte h. space ; (  - )

/* example use

  fload
  464 bottom + next

  \ without fload:
  464 536870912 + next

*/

/*

  good-comp-tstorm-hah
  good-compiler-aa-dd
  good-compiler-aa-ee
  good-compiler-aa-ff
* good-compiler-aa-ff-aa_exp
  master

All the above are recently related to each other,
along with 'develop'.

Current branch is now  good-compiler-aa-ff-aa_exp
and was created with a

 $ git checkout -b  from   good-compiler-aa-ff

Sat Aug 24 17:55:00 UTC 2019

*/

/*
 $ git branch
* good-comp-tstorm-hah - properly pulled and manually updated using rvim and brain ;) (for a change)
good-compiler-aa-ee - recent work - may already be merged, or abandoned.  Not sure where first instances are.

// also:
// Sat Aug 24 15:32:03 UTC 2019 - oops stuff fixing

*/


extern void _dumpRAM(void);
extern void _getOneByteRAM(void); // ( addr -- )

// On branch  good-compiler-aa-bb

// identify: kibarthe   tr0mso   cablefour  entwistle  pescanole

// target: ItsyBitsy M4 Express - still current on branch  good-compiler-aa-bb  21 August 2019
// comm: USB, not the TX/RX pair for the Forth interpreter - on branch  good-compiler-aa-bb

// terminal: minicom (provides keystroke echo) (and color support)
// the other method is to construct a 'terminal' from a Trinket M0 and use the UART ;)

// Note: other branches may want to use the UART rather than USB.

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
24 August 2019 15:54 UTC
.s
empty  Ok
fload
 loading a forth program from flashROM ..
/forth/ascii_xfer_a001.txt was closed - Cortex-Forth.ino LINE 369
.s
empty  Ok
464 bottom + dup rlist
200001D0 : 00 00 00 00 D1 42 00 00 04 65 78 69 14 00 00 00   .....B...exi....
200001E0 : D5 42 00 00 03 6B 65 79 17 00 00 00 75 5A 00 00   .B...key....uZ..
200001F0 : 04 65 6D 69 1A 00 00 00 99 5A 00 00 02 63 72 00   .emi.....Z...cr.

session using the 'cc' (compose) word:
 Ok
cc
 61  a 62  b 63  c 64  d 65  e 66  f 67  g F   Ctrl+O pressed  F   Ctrl+O pressed  A  
 A  
 A  
 2E  . 73  s A  

*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//   In theory, sfparse can be called just the same as flparse is
//   called, at present.  They're identical (only at the moment)
//   (13 Aug 03:23z).

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

/*
  Forth virtual machine

  This code is in the public domain.

*/
#include <SdFat.h> // 'File' 10 Aug 16:10z

// 0x1200 == 4608 decimal

#define RAM_SIZE 0x1200
#define S0 0x1000
#define R0 0x0f00
#define NAME(m, f, c, x, y, z) {memory.data [m] = f + c + (x << 8) + (y << 16) + (z << 24);}
#define LINK(m, a) {memory.data [m] = a;}
#define CODE(m, a) {memory.program [m] = a;}
#define DATA(m, a) {memory.data [m] = a;}
#define IMMED 0x80

#include "prequel.h"
#include "compatibility.h"

#include "common.h"

#define LINE_ENDING 10 // alt: 13

#ifdef HAS_DOTSTAR_LIB
extern void setup_dotstar(void); // dotstar.cpp
extern void set_dotStarColors(void);
extern void wiggleDSManyTimes(void);
extern void wiggleDotStarOnce(void); // toggle once
#endif // #ifdef HAS_DOTSTAR_LIB


extern void fl_setup(void); // fload.cpp
extern void flash_setup(void); // flash_ops.cpp

extern File thisFile; // You must include SdFat.h to use 'File' here

// global variables
union Memory {
  int data [RAM_SIZE];
  void (*program []) (void);
} memory;

String tib = "";
int S = S0; // data stack pointer
int R = R0; // return stack pointer
int I = 0; // instruction pointer
int W = 0; // working register
int T = 0; // top of stack
int H = 0; // dictionary pointer, HERE
int D = 0; // dictionary list entry point
int base = 10;
boolean state = false; // compiling or not
boolean keyboard_not_file = true; // keyboard or file input, for parsing

/*  A word in the dictionary has these fields:
  name  32b word,  a 32 bit int, made up of byte count and three letters
  link  32b word, point to next word in list, 0 says end of list
  code  32b word, a pointer to some actual C compiled code,
        all native code is in this field
  data  32b word, at least, a list to execute or a data field of some kind


*/

// primitive definitions

void _LBRAC (void) {
  state = false; // interpreting
}

void _RBRAC (void) {
  state = true; // compiling
}

void _NOP (void) {
  return;
}

void _FLOAD (void) { // file load: fload
  SERIAL_LOCAL_C.print(" loading a forth program from flashROM ..\r");
     I = 190; //  simulate 'quit'  - does not clear the stack. I = 83 (abort) does.
  // I = 82; //  allows typing but never exits (infinite nesting?)
  // I = 83; //  simulate 'abort' - this 83 is a #define later on.
}

void _WAGDS (void) { // 'wag' the dotStar colored lED - ItsyBitsy M4, others
#ifdef HAS_DOTSTAR_LIB
  wiggleDotStarOnce();
#endif // #ifdef HAS_DOTSTAR_LIB
}

void _WIGGLE (void) { // toggle dotStar a number of times
/*
 124 void _EMIT (void) {
 125   char c = T;
 126   SERIAL_LOCAL_C.write (c);
 127   _DROP ();
 128 }
*/
  for (int i = T; i > 0; i--) {
    _WAGDS();
  }
  _DROP ();
  // wiggleDotStarOnce();
}

void _EXIT (void) {
  I = memory.data [R++];
}

void _DROP (void) {
  T = memory.data [S++];
}

void _DUP (void) {
  memory.data [--S] = T;
}

void _QDUP (void) {
  if (T) _DUP ();
}

void _KEY (void) {
  _DUP ();
  while (!SERIAL_LOCAL_C.available ());
  T = SERIAL_LOCAL_C.read ();
//  SERIAL_LOCAL_C.write (T);
}

void _EMIT (void) {
  char c = T;
  SERIAL_LOCAL_C.print (c);
  _DROP ();
}

void _CR (void) {
  char c = '\r';
  SERIAL_LOCAL_C.write (c);
  // SERIAL_LOCAL_C.write ("\r");
}

void _OK (void) {
  if (tib [tib.length () - 1] == LINE_ENDING) SERIAL_LOCAL_C.print (" Ok\r");
}

void _WLIST (void) {
  SERIAL_LOCAL_C.print ("wiggle wag fload wlist warm type c! c@ literal repeat while again ' forget else then if until begin loop do i ; : ] [ R constant ? variable allot here create dump 2/ 2* negate abs invert xor or and - + h. space words .s . flparse quit 0< depth number ?dup execute find , ! @ over swap drop dup word parse cr emit key exit ");
}

void _WARM (void) {
  NVIC_SystemReset();      // processor software reset
}

void _COMPOSE (void) {
  while(-1) { // always true
    _KEY();
    _SPACE(); _DUP(); _HDOT(); _SPACE();
    if (T == 1) SERIAL_LOCAL_C.print(" Ctrl+A pressed ");
    if (T == 2) SERIAL_LOCAL_C.print(" Ctrl+B pressed ");
    if (T == 7) SERIAL_LOCAL_C.print(" Ctrl+G BELL pressed ");
    if (T == 8) SERIAL_LOCAL_C.print(" Ctrl+H BACKSPACE pressed ");
    if (T == 15) SERIAL_LOCAL_C.print(" Ctrl+O pressed ");
    if (T == 27) SERIAL_LOCAL_C.print(" ESC pressed ");
    // if (T == 127) SERIAL_LOCAL_C.print(" RUBOUT pressed \(0x7f\) ");
    if (T == 127) SERIAL_LOCAL_C.print(" RUBOUT pressed 0x7f ");
    _EMIT();
  }
}

void _RBYTE (void) { // _getOneByteRAM(); // ( addr -- )
  _getOneByteRAM();
}

void _SWAP (void) {
  W = memory.data [S];
  memory.data [S] = T;
  T = W;
}

void _OVER (void) {
  _DUP ();
  T = memory.data [S + 1];
}

void _FETCH (void) {
  T = memory.data [T];
}

void _STORE (void) {
  W = T,
  _DROP ();
  memory.data [W] = T;
  _DROP ();
}

void _COMMA (void) {
  memory.data [H++] = T;
  _DROP ();
}

void _MINUS (void) {
  W = T;
  _DROP ();
  T = (T - W);
}

void _PLUS (void) {
  W = T;
  _DROP ();
  T = (T + W);
}

void _aND (void) {
  W = T;
  _DROP ();
  T = (T & W);
}

void _OR (void) {
  W = T;
  _DROP ();
  T = (T | W);
}

void _XOR (void) {
  W = T;
  _DROP ();
  T = (T ^ W);
}

void _INVERT (void) {
  T = ~T;
}

void _ABS (void) {
  T = abs (T);
}

void _NEGATE (void) {
  T = -T;
}

void _TWOSLASH (void) {
  T = (T >> 1);
}

void _TWOSTAR (void) {
  T = (T << 1);
}

void _LIT (void) {
  _DUP (); 
  T = memory.data [I++];
}

void _BRANCH (void) {
  I = memory.data [I];
}

void _0BRANCH (void) {
  if (T == 0) {
    I = memory.data [I];
    _DROP ();
    return;
  }
  I += 1;
  _DROP ();
}

void _INITR (void) {
  R = R0;
}

void _INITS (void) {
  S = S0;
}

void _NEST (void) {
  memory.data [--R] = I;
  I = (W + 1);
}

void _SHOWTIB (void) {
  W = tib.length ();
  tib [W - 1] = 0;
  SERIAL_LOCAL_C.print (tib); // tnr // restored to original
}

// trim leading spaces
void _PARSE (void) {
  char t;
  tib = "";
  do {
    while (!SERIAL_LOCAL_C.available ());
    t = SERIAL_LOCAL_C.peek ();
    if (t == ' ') {
      t = SERIAL_LOCAL_C.read ();
//      SERIAL_LOCAL_C.print (t);
    }
  } while (t == ' ');
  do {
    while (!SERIAL_LOCAL_C.available ());
    t = SERIAL_LOCAL_C.read ();

#ifdef ECHO_INPUT
    SERIAL_LOCAL_C.print (t);
#endif

    tib = tib + t;
  } while (t > ' ');
  // tnr, suppressed // SERIAL_LOCAL_C.print (tib);
}

// trim leading spaces
// parse .. and right now, flparse .. is the only
// place that acquires input, other than _KEY ..
// and _KEY is not used internally (at all!)

// This might make for a neat solution to the
// problem of reading from a file, as only this
// one (C language) function seems to talk to the
// serial port, to read from it.

// All other references to the serial port involve
// writing to it (printing messages to the serial
// terminal).


// change from 3 to 2 - 11 Aug 17:41z
// change from 2 back to 3 - file isn't closing at the right time.

// 12 Aug 23:42 - way too sensitive manual test for EOF here:

// This Forth does NOT like println() to the file; it wants 'print("foo \r");

#define FLEN_MAX 2
void _FLPARSE (void) {
  char t;
  tib = "";
  keyboard_not_file = false;
  if (thisFile) {
    while (thisFile.available() > FLEN_MAX) { // new conditional 17:25z
      do {
        t = thisFile.read();
        char peeked_char = t;
        tib = peeked_char; // not sure where this goes
        if (t == ' ') {
          tib = ""; // unpeek tib
          t = thisFile.read();
        }
      } while (t == ' ');
      do {
        t = thisFile.read();
          tib = tib + t; // was unconditional before 19:01z 10 Aug
      } while (t > ' ');
      if (thisFile.available() < (FLEN_MAX - 1)) {
        if (thisFile.available() < (1)) {
          keyboard_not_file = true;
          thisFile.close(); // experiment 17:06z 11 Aug
          SERIAL_LOCAL_C.print('\r');
          SERIAL_LOCAL_C.print(FILE_NAME);
          SERIAL_LOCAL_C.print(" was closed - Cortex-Forth.ino LINE 369\r");
        }
      }
      return; // EXPERIMENT - this could crash it - not sure why but the TRAP lines are ignored in Forth - but the very last line was not ignored and made it onto the stack (it was a pushed value).
    } // new conditional 17:25z
    SERIAL_LOCAL_C.print(" alt TRAP LINE 334\r");
    delay(1400); // KLUDGE tnr kludge 12 Aug 23:15
  } // if thisfile
  else {
    keyboard_not_file = true;
    I = 90; // I = 90 points to 'parse' - top of original quit loop
  }
}

// #define FLEN_MAX 2
void _SFPARSE (void) { // safe parse
  char t;
  tib = "";
  keyboard_not_file = false;
  if (thisFile) {
    while (thisFile.available() > FLEN_MAX) { // new conditional 17:25z
      do {
        t = thisFile.read();
        char peeked_char = t;
        tib = peeked_char; // not sure where this goes
        if (t == ' ') {
          tib = ""; // unpeek tib
          t = thisFile.read();
        }
      } while (t == ' ');
      do {
        t = thisFile.read();
          tib = tib + t; // was unconditional before 19:01z 10 Aug
      } while (t > ' ');
      if (thisFile.available() < (FLEN_MAX - 1)) {
        if (thisFile.available() < (1)) {
          keyboard_not_file = true;
          thisFile.close(); // experiment 17:06z 11 Aug
          SERIAL_LOCAL_C.print("\r");
          SERIAL_LOCAL_C.print(FILE_NAME);
          SERIAL_LOCAL_C.print(" was closed - Cortex-Forth.ino LINE 347\r");
        }
      }
      return; // EXPERIMENT - this could crash it - not sure why but the TRAP lines are ignored in Forth - but the very last line was not ignored and made it onto the stack (it was a pushed value).
    } // new conditional 17:25z
    SERIAL_LOCAL_C.print(" alt TRAP LINE 334\r");
    delay(1400); // KLUDGE tnr kludge 12 Aug 23:15
  } // if thisfile
  else {
    keyboard_not_file = true;
    I = 90; // I = 90 points to 'parse' - top of original quit loop
  }
}

void _WORD (void) {
  char t;
  _DUP (); // what are we dup'ing here
  T = (tib.length () - 1);
  W = T;
  t = tib [0];

  // looks like tib needs the follow-on character here

  T |= (t << 8);
  if (W > 1) {
    t = tib [1];
    T |= (t << 16);
  }
  if (W > 2) {
    t = tib [2];
    T |= (t << 24);
  }
  // SERIAL_LOCAL_C.print(" ~k~ ");
  // SERIAL_LOCAL_C.print(" --- _WORD  exits --- \r");
}

void _NUMBER (void) {
  char t;
  _DUP ();
  T = 0;
  for (int i = 0; i < (tib.length () - 1); i++) {
    if (i == 0) {
      if (tib [i] == '-') continue;
    }
    t = tib [i];
    if (!isDigit (t)) {
    if (tib [0] == '-') T = -T;
      _DUP ();
      T = -1;
      return;
    }
    T *= base;
    t -= '0';
    if (t > 9) t -= 37;
    T += t;
  }
  if (tib [0] == '-') T = -T;
      if (state == true) {
        _DUP ();
        T = 1; // forward reference to lit
        _COMMA (); // lit
        _COMMA (); // the number
      }
  _DUP ();
  T = 0;
}

void _EXECUTE (void) {
  if (state == true) {
    if (((memory.data [T]) & 0x80) == 0) {
      T += 2;
      _COMMA ();
      return;
    }
  }
  W = (T + 2);
  _DROP ();
  memory.program [W] ();
}

void _FIND (void) {
  int X = T;
  T = D;
  while (T != 0) {
    W = (memory.data [T]);
    if ((W & 0xffffff7f) == X) {
      // SERIAL_LOCAL_C.print("FIND exits - and its a word.\r");
      return;
    }
    T = memory.data [T + 1];
  }
  // SERIAL_LOCAL_C.print("FIND exits.\r");
}

void _DOT (void) {
  SERIAL_LOCAL_C.print (T);
  SERIAL_LOCAL_C.print (' ');
  _DROP ();
}

void _HDOT (void) {
  SERIAL_LOCAL_C.print (T, HEX);
  SERIAL_LOCAL_C.print (' ');
  _DROP ();
}

// LESSON LEARNED: when it was 'write' not 'print' the "empty " was not printed,
// but the spaces that made it up were (probably non-printing chars).
void _DDOTS (void) {
  if (S == S0) {
    SERIAL_LOCAL_C.print ("empty ");
    return;
  }
  _DUP ();
  W = (S0 - 1);
  while (W > (S)) {
    SERIAL_LOCAL_C.print (memory.data [--W]);
    SERIAL_LOCAL_C.print (' ');
  }
  _DROP ();
}

void _SPACE () {
  SERIAL_LOCAL_C.print (' ');
}

void _ZEROEQUAL () {
  if (T == 0) {
    T = -1;
    return;
  }
  T = 0;
}

void _ZEROLESS () {
  if (T < 0) {
    T = -1;
    return;
  }
  T = 0;
}

void _DOTWORD () {
  int Y = memory.data [W];
  int X = (Y & 0xff);
  SERIAL_LOCAL_C.print ('[');
  SERIAL_LOCAL_C.print (X);
  SERIAL_LOCAL_C.print (' ');
  X = ((Y >> 8) & 0xff);
  _DUP (); T = X; _EMIT ();
  X = ((Y >> 16) & 0xff);
  if (X != 0) { _DUP (); T = X; _EMIT (); }
  X = ((Y >> 24) & 0xff);
  if (X != 0) { _DUP (); T = X; _EMIT (); }
  // SERIAL_LOCAL_C.print ("] "); 
  SERIAL_LOCAL_C.print ("] "); 
  // delay(20); // wild guess kludge tnr 26 August - for 9term only
  // want to stop flooding 9term with too much information sent
  // to it too quickly.
}

void _WORDS (void) {
  int i = 7;
  W = D;
  do {
    _DOTWORD ();
    W = memory.data [++W];
    i += 1;
// ###bookmark
    if ((i % 8) == 0) _CR ();
  } while (memory.data [W + 1]);
}

void _DEPTH (void) {
  W = S0 - S;
  _DUP ();
  T = W;
}

void _DUMP (void) {
  int a = T;
  _DROP ();
  for (int i = 0; i < a; i++) {
    W = T;
    SERIAL_LOCAL_C.print (memory.data [T++], HEX);
    // SERIAL_LOCAL_C.print (' ');
    SERIAL_LOCAL_C.print (" ~dump_delimiter~ ");
    _DOTWORD ();
  }
}

void _HERE (void) {
  _DUP ();
  T = H;
}

void _ALLOT (void) {
  H += T;
  _DROP ();
}

void _HEAD (void) {
  if ( keyboard_not_file ) {
    _PARSE ();
  } else {
    _FLPARSE ();
  }
//  _PARSE ();
  _WORD ();
  _COMMA ();
  _DUP ();
  T = D;
  _COMMA ();
  D = H - 2;
}

void _DOVAR (void) {
  _DUP ();
  T = (W + 1);
}

void _CREATE (void) {
  _HEAD ();
  _DUP ();
  _DUP ();
  memory.program [S] = _DOVAR;
  _DROP ();
  _COMMA ();
}

void _COLON (void) {
  _HEAD ();
  _DUP ();
  _DUP ();
  memory.program [S] = _NEST;
  _DROP ();
  _COMMA ();
  _RBRAC ();
}

void _SEMI (void) {
  _DUP ();
  T = 25; // forward reference to exit 
  _COMMA (); // compile exit
  _LBRAC (); // stop compiling
}

void _DOCONST (void) {
  _DUP ();
  T = memory.data [W + 1];
}

void _CONSTANT (void) {
  _HEAD ();
  _DUP ();
  _DUP ();
  memory.program [S] = _DOCONST;
  _DROP ();
  _COMMA ();
  _COMMA ();
}

void _VARIABLE (void) {
  _CREATE ();
  H += 1;
}

void _QUESTION (void) {
  _FETCH ();
  _DOT ();
}

void _R (void) {
  _DUP ();
  T = R;
}

void _DO (void) {
  memory.data [--R] = T;
  _DROP ();
  memory.data [--R] = T;
  _DROP ();
}

void _LOOP (void) {
  int X = memory.data [R++];
  W = (memory.data [R++] + 1);
  if (W == X) {
    I += 1;
    return;
  }
  memory.data [--R] = (W);
  memory.data [--R] = X;
  I = memory.data [I];
}

void _I (void) {
  _DUP ();
  T = memory.data [R + 1];
}

void _CDO (void) {
  _DUP ();
  T = 4; // forward reference to ddo
  _COMMA ();
  _DUP ();
  T = H;
}

void _CLOOP (void) {
  _DUP ();
  T = 5; // forward reference to lloop
  _COMMA ();
  _COMMA (); // address left on stack by do
}

void _CBEGIN (void) {
  _DUP ();
  T = H;
}

void _CUNTIL (void) {
  _DUP ();
  T = 3; // forward reference to Obranch
  _COMMA ();
  _COMMA (); // address left on stack by begin
}

void _CAGAIN (void) {
  _DUP ();
  T = 2; // forward reference to branch
  _COMMA ();
  _COMMA (); // address left on stack by begin
}

void _CIF (void) {
  _DUP ();
  T = 3; // forward reference to 0branch
  _COMMA ();
  _DUP ();
  T = H; // address that needs patching later
  _DUP ();
  T = 0;
  _COMMA (); // dummy in address field
}

void _CWHILE (void) {
  _CIF ();
  _SWAP ();
}

void _CTHEN (void) {
  _DUP ();
  T = H;
  _SWAP ();
  _STORE ();
}

void _CREPEAT (void) {
  _CAGAIN ();
  _CTHEN ();
}

void _CELSE (void) {
  _DUP ();
  T = 2; // forward reference to branch
  _COMMA ();
  _DUP ();
  T = H; // address that needs patching later
  _DUP ();
  T = 0;
  _COMMA (); // dummy in address field
  _SWAP ();
  _CTHEN ();
}

void _FORGET (void) {
  _PARSE ();
  _WORD ();
  _FIND ();
  D = memory.data [T + 1];
  H = T;
  _DROP ();
}

void _TICK (void) {
  SERIAL_LOCAL_C.print("WHOOPS - _TICK encountered! \r");
  _PARSE ();
  _WORD ();
  _FIND ();
}

void _CLITERAL (void) {
  _DUP ();
  T = 1; // forward reference to lit
  _COMMA ();
  _COMMA (); // the number that was already on the stack
}

void _CFETCH (void) {
  W = (T % 4);
  T = memory.data [T / 4];
  T = (T >> (W * 8) & 0xff);
} 

void _CSTORE (void) {
  int X = (T / 4);
  W = (T % 4);
  _DROP ();
  T = (T << (W * 8));
  T = (T | (memory.data [X] & ~(0xff << (W * 8))));
  memory.data [X] = T;
  _DROP ();
} 


void _color_yellow_fg (void) {
  SERIAL_LOCAL_C.print("\033\133"); // ESC [
  SERIAL_LOCAL_C.print("\063\063"); // 33 - yellow fg
  SERIAL_LOCAL_C.print("m");        // for the stanza
}

void _color_blue_bg (void) {
  SERIAL_LOCAL_C.print("\033\133"); // ESC [
  SERIAL_LOCAL_C.print("\064\064"); // 44 - blue bg
  SERIAL_LOCAL_C.print("m");        // for the stanza
}

void _color_black_bg (void) {
  SERIAL_LOCAL_C.print("\033\133"); // ESC [
  SERIAL_LOCAL_C.print("\064\060"); // 40 - black bg
  SERIAL_LOCAL_C.print("m");        // for the stanza
}


void setup () {
#ifdef HAS_DOTSTAR_LIB
  setup_dotstar(); // turn off dotstar (apa-102 RGB LED)
  // set_dotStarColors(); // give them some color
  // wiggleDSManyTimes(); // repeat the toggle
#endif // #ifdef HAS_DOTSTAR_LIB

  // SERIAL_LOCAL_C.begin (38400); while (!SERIAL_LOCAL_C);

  S = S0; // initialize data stack
  R = R0; // initialize return stack

  // initialize dictionary

  // unlinked primitives
  CODE(1, _LIT)
#  define lit 1
  CODE(2, _BRANCH)
#  define branch 2
  CODE(3, _0BRANCH)
#  define zbranch 3
  CODE(4, _DO)
#  define ddo 4
  CODE(5, _LOOP)
#  define lloop 5
  CODE(6, _INITR)
#  define initr 6
  CODE(7, _INITS)
#  define inits 7
  CODE(8, _SHOWTIB)
#  define showtib 8
  CODE(9, _OK)
#  define ok 9
  // room to expand here

  // trailing space kludge
  NAME(20, 0, 0, 10, 0, 0)
  LINK(21, 0)
  CODE(22, _NOP)
#define nop 22
  // exit
  NAME(23, 0, 4, 'e', 'x', 'i')
  LINK(24, 20)
  CODE(25, _EXIT)
#  define exit 25
  // key ( - c)
  NAME(26, 0, 3, 'k', 'e', 'y')
  LINK(27, 23)
  CODE(28, _KEY)
  // emit ( c - )
  NAME(29, 0, 4, 'e', 'm', 'i')
  LINK(30, 26)
  CODE(31, _EMIT)
#  define emit 31
  // cr
  NAME(32, 0, 2, 'c', 'r', 0)
  LINK(33, 29)
  CODE(34, _CR)
#  define cr 34
  // parse // leaves string in tib
  NAME(35, 0, 5, 'p', 'a', 'r')
  LINK(36, 32)
  CODE(37, _PARSE)
#  define parse 37
  // word ( - n) gets string from tib
  NAME(38, 0, 4, 'w', 'o', 'r')
  LINK(39, 35)
  CODE(40, _WORD)
#  define wword 40
  // dup ( n - n n)
  NAME(41, 0, 3, 'd', 'u', 'p')
  LINK(42, 38)
  CODE(43, _DUP)
#  define dup 43
  // drop ( n - )
  NAME(44, 0, 4, 'd', 'r', 'o')
  LINK(45, 41)
  CODE(46, _DROP)
#  define drop 46
  // swap ( n1 n2 - n2 n1)
  NAME(47, 0, 4, 's', 'w', 'a')
  LINK(48, 44)
  CODE(49, _SWAP)
#  define swap 49
  // over ( n1 n2 - n1 n2 n1)
  NAME(50, 0, 4, 'o', 'v', 'e')
  LINK(51, 47)
  CODE(52, _OVER)
#  define over 52
  // @ ( a - n)
  NAME(53, 0, 1, '@', 0, 0)
  LINK(54, 50)
  CODE(55, _FETCH)
  // ! ( n a - )
  NAME(56, 0, 1, '!', 0, 0)
  LINK(57, 53)
  CODE(58, _STORE)
  // , ( n - )
  NAME(59, 0, 1, ',', 0, 0)
  LINK(60, 56)
  CODE(61, _COMMA)
  // find ( n - a)
  NAME(62, 0, 4, 'f', 'i', 'n')
  LINK(63, 59)
  CODE(64, _FIND)
#  define find 64
  // execute ( a)
  NAME(65, 0, 7, 'e', 'x', 'e')
  LINK(66, 62)
  CODE(67, _EXECUTE)
#  define execute 67
  // ?dup ( n - 0 | n n)
  NAME(68, 0, 3, '?', 'd', 'u')
  LINK(69, 65)
  CODE(70, _QDUP)
#  define qdup 70
  // number ( - n -f) gets string from tib
  NAME(71, 0, 6, 'n', 'u', 'm')
  LINK(72, 68)
  CODE(73, _NUMBER)
#  define number 73
  // depth ( - n)
  NAME(74, 0, 5, 'd', 'e', 'p')
  LINK(75, 71)
  CODE(76, _DEPTH)
#  define depth 76
  // 0< ( n - f)
  NAME(77, 0, 2, '0', '<', 0)
  LINK(78, 74)
  CODE(79, _ZEROLESS)
#  define zeroless 79

  // abort
  NAME(80, 0, 5, 'a', 'b', 'o')
  LINK(81, 77)
  CODE(82, _NEST)
  DATA(83, inits)
#  define abort 83
  // again
  DATA(84, branch)
  DATA(85, 89)
  // quit
  NAME(86, 0, 4, 'q', 'u', 'i')
  LINK(87, 77)
  CODE(88, _NEST)
  DATA(89, initr)
  // begin quit loop
  DATA(90, parse)
  DATA(91, wword)
  DATA(92, find)
  DATA(93, qdup)
  DATA(94, zbranch)
  DATA(95, 103) // to number
  DATA(96, execute)
  DATA(97, depth)
  DATA(98, zeroless)
  DATA(99, zbranch)
  DATA(100, 114) // to ok
  DATA(101, branch)
  DATA(102, 106)
  DATA(103, number)
  DATA(104, zbranch)
  DATA(105, 114) // to ok
  DATA(106, nop) // tnr, suppressed with a nop // DATA(106, showtib)
  DATA(107, lit)
  DATA(108, '?')
  DATA(109, emit)
  DATA(110, cr)
  DATA(111, inits)
  DATA(112, branch)
  DATA(113, 89)
  DATA(114, ok)
  DATA(115, branch)
  DATA(116, 90) // continue quit loop

  // a 'branch' points to a 'DATA' statement

  // - - - - -   large  gap  here   - - - - -

  // flparse // leaves string in tib
  NAME(135, 0, 7, 'f', 'l', 'p')
  LINK(136, 86)
  CODE(137, _FLPARSE)
#  define flparse 137

  // sfparse // leaves string in tib
  NAME(138, 0, 7, 's', 'f', 'p')
  LINK(139, 135)
  CODE(140, _SFPARSE)
#  define sfparse 140


  // flabort
  NAME(180, 0, 7, 'f', 'l', 'a')
  LINK(181, 77)
  CODE(182, _NEST)
  DATA(183, inits)
#  define flabort 183
  // fload loop - again
  DATA(184, branch)
  DATA(185, 189)
  // bye - seems ignored
  NAME(186, 0, 3, 'b', 'y', 'e')
  LINK(187, 77) // 0< - may be the same entry point
  CODE(188, _NEST)
  DATA(189, initr)
  // begin quit loop
  DATA(190, flparse) // latest change
  DATA(191, wword) // gets string from tib
  DATA(192, find)
  DATA(193, qdup)
  DATA(194, zbranch)
  DATA(195, 203) // to number
  DATA(196, execute)
  DATA(197, depth)
  DATA(198, zeroless)
  DATA(199, zbranch)
  DATA(200, 214) // to ok
  DATA(201, branch)
  DATA(202, 206)
  DATA(203, number)
  DATA(204, zbranch)
  DATA(205, 214) // to ok
  DATA(206, nop) // tnr, suppressed with a nop // DATA(106, showtib)
  DATA(207, lit)
  DATA(208, '~') // was '?' in the original
  DATA(209, emit)
  DATA(210, cr)
  DATA(211, inits)
  DATA(212, branch)
  DATA(213, 189)
  DATA(214, ok)
  DATA(215, branch)
  DATA(216, 190) // continue quit loop





  // sfabort
  NAME(280, 0, 7, 's', 'f', 'a')
  LINK(281, 77)
  CODE(282, _NEST)
  DATA(283, inits)
#  define sfabort 283
  // sfload loop - again
  DATA(284, branch)
  DATA(285, 289)
  // alhoa - seems ignored
  NAME(286, 0, 5, 'a', 'l', 'o')
  LINK(287, 77) // 0< - may be the same entry point
  CODE(288, _NEST)
  DATA(289, initr)
  // begin quit loop
  DATA(290, sfparse) // latest change - - - - - - - - - - - - - - - -
  DATA(291, wword) // gets string from tib
  DATA(292, find)
  DATA(293, qdup)
  DATA(294, zbranch)
  DATA(295, 303) // to number
  DATA(296, execute)
  DATA(297, depth)
  DATA(298, zeroless)
  DATA(299, zbranch)
  DATA(300, 314) // to ok
  DATA(301, branch)
  DATA(302, 306)
  DATA(303, number)
  DATA(304, zbranch)
  DATA(305, 314) // to ok
  DATA(306, nop) // tnr, suppressed with a nop // DATA(106, showtib)
  DATA(307, lit)
  DATA(308, '!') // was '~' in the recent copy of the original
  DATA(309, emit)
  DATA(310, cr)
  DATA(311, inits)
  DATA(312, branch)
  DATA(313, 289)
  DATA(314, ok)
  DATA(315, branch)
  DATA(316, 290) // continue quit loop



  // . ( n - )
  NAME(317, 0, 1, '.', 0, 0)
  LINK(318, 138) // 135 // 86 // if this isn't 86 then the quit word is lost -- old: NEAT - links back to the quit word here
  CODE(319, _DOT)
#  define dot 319
  // .s
  NAME(320, 0, 2, '.', 's', 0)
  LINK(321, 317)
  CODE(322, _DDOTS)
#  define ddots 322
  // words
  NAME(323, 0, 5, 'w', 'o', 'r')
  LINK(324, 320)
  CODE(325, _WORDS)
#  define words 325
  // space
  NAME(326, 0, 5, 's', 'p', 'a')
  LINK(327, 323)
  CODE(328, _SPACE)
#  define space 328
  // h. ( n - )
  NAME(329, 0, 2, 'h', '.', 0)
  LINK(330, 326)
  CODE(331, _HDOT)
#  define hdot 331
  // + ( n1 n2 - n3)
  NAME(332, 0, 1, '+', 0, 0)
  LINK(333, 329)
  CODE(334, _PLUS)
#  define plus 334
  // - ( n1 n2 - n3)
  NAME(335, 0, 1, '-', 0, 0)
  LINK(336, 332)
  CODE(337, _MINUS)
  // and (n1 n2 - n3)
  NAME(338, 0, 3, 'a', 'n', 'd')
  LINK(339, 335)
  CODE(340, _aND)
  // or ( n1 n2 - n3)
  NAME(341, 0, 2, 'o', 'r', 0)
  LINK(342, 338)
  CODE(343, _OR)
  // xor ( n1 n2 - n3)
  NAME(344, 0, 3, 'x', 'o', 'r')
  LINK(345, 341)
  CODE(346, _XOR)
  // invert ( n1 - n2)
  NAME(347, 0, 6, 'i', 'n', 'v')
  LINK(348, 344)
  CODE(349, _INVERT)
  // abs ( n1 - n2)
  NAME(350, 0, 3, 'a', 'b', 's')
  LINK(351, 347)
  CODE(352, _ABS)
  // negate ( n1 - n2)
  NAME(353, 0, 6, 'n', 'e', 'g')
  LINK(354, 350)
  CODE(355, _NEGATE)
  // 2* ( n1 - n2)
  NAME(356, 0, 2, '2', '*', 0)
  LINK(357, 353)
  CODE(358, _TWOSTAR)
  // 2/ ( n1 - n2)
  NAME(359, 0, 2, '2', '/', 0)
  LINK(360, 356)
  CODE(361, _TWOSLASH)
  // dump ( a n - a+n) 
  NAME(362, 0, 4, 'd', 'u', 'm')
  LINK(363, 359)
  CODE(364, _DUMP)
  // create
  NAME(365, 0, 6, 'c', 'r', 'e')
  LINK(366, 362)
  CODE(367, _CREATE)
  // here 
  NAME(368, 0, 4, 'h', 'e', 'r')
  LINK(369, 365)
  CODE(370, _HERE)
  // allot 
  NAME(371, 0, 5, 'a', 'l', 'l')
  LINK(372, 368)
  CODE(373, _ALLOT)
  // variable 
  NAME(374, 0, 8, 'v', 'a', 'r')
  LINK(375, 371)
  CODE(376, _VARIABLE)
  // ?
  NAME(377, 0, 1, '?', 0, 0)
  LINK(378, 374)
  CODE(379, _QUESTION)
  // constant
  NAME(380, 0, 8, 'c', 'o', 'n')
  LINK(381, 377)
  CODE(382, _CONSTANT)
  // R
  NAME(383, 0, 1, 'R', 0, 0)
  LINK(384, 380)
  CODE(385, _R)
  // [ 
  NAME(386, IMMED, 1, '[', 0, 0)
  LINK(387, 383)
  CODE(388, _LBRAC)
  // ]
  NAME(389, 0, 1, ']', 0, 0)
  LINK(390, 386)
  CODE(391, _RBRAC)
  // :
  NAME(392, 0, 1, ':', 0, 0)
  LINK(393, 389)
  CODE(394, _COLON)
  // ;
  NAME(395, IMMED, 1, ';', 0, 0)
  LINK(396, 392)
  CODE(397, _SEMI)
  // i 
  NAME(398, 0, 1, 'i', 0, 0)
  LINK(399, 395)
  CODE(400, _I)
#  define _i 400
  // do
  NAME(401, IMMED, 2, 'd', 'o', 0)
  LINK(402, 398)
  CODE(403, _CDO)
  // loop 
  NAME(404, IMMED, 4, 'l', 'o', 'o')
  LINK(405, 401)
  CODE(406, _CLOOP)
  // begin 
  NAME(407, IMMED, 5, 'b', 'e', 'g')
  LINK(408, 404)
  CODE(409, _CBEGIN)
  // until 
  NAME(410, IMMED, 5, 'u', 'n', 't')
  LINK(411, 407)
  CODE(412, _CUNTIL)
  // if
  NAME(413, IMMED, 2, 'i', 'f', 0)
  LINK(414, 410)
  CODE(415, _CIF)
  // then
  NAME(416, IMMED, 4, 't', 'h', 'e')
  LINK(417, 413)
  CODE(418, _CTHEN)
  // else
  NAME(419, IMMED, 4, 'e', 'l', 's')
  LINK(420, 416)
  CODE(421, _CELSE)
  // forget
  NAME(422, 0, 6, 'f', 'o', 'r')
  LINK(423, 419)
  CODE(424, _FORGET)
  // '
  NAME(425, 0, 1, '\'', 0, 0)
  LINK(426, 422)
  CODE(427, _TICK)
  // again
  NAME(428, IMMED, 5, 'a', 'g', 'a')
  LINK(429, 425)
  CODE(430, _CAGAIN)
  // while
  NAME(431, IMMED, 5, 'w', 'h', 'i')
  LINK(432, 428)
  CODE(433, _CWHILE)
  // repeat
  NAME(434, IMMED, 6, 'r', 'e', 'p')
  LINK(435, 431)
  CODE(436, _CREPEAT)
  // literal 
  NAME(437, IMMED, 7, 'l', 'i', 't')
  LINK(438, 434)
  CODE(439, _CLITERAL)
  // c@ ( b - c) 
  NAME(440, 0, 2, 'c', '@', 0)
  LINK(441, 437)
  CODE(442, _CFETCH)
#  define cfetch 442
  // c! ( c b - ) 
  NAME(443, 0, 2, 'c', '!', 0)
  LINK(444, 440)
  CODE(445, _CSTORE)
  // type ( b c - ) 
  NAME(446, 0, 4, 't', 'y', 'p')
  LINK(447, 443)
  CODE(448, _NEST)
  DATA(449, over)
  DATA(450, plus)
  DATA(451, swap)
  DATA(452, ddo)
  DATA(453, _i)
  DATA(454, cfetch)
  DATA(455, emit)
  DATA(456, lloop)
  DATA(457, 453)
  DATA(458, exit)
  // warm (  - )
  NAME(459, 0, 4, 'w', 'a', 'r')
  LINK(460, 446)
  CODE(461, _WARM)

  // wlist (  - )
  NAME(462, 0, 5, 'w', 'l', 'i')
  LINK(463, 459)
  CODE(464, _WLIST)

  // fload (  - )
  NAME(465, 0, 5, 'f', 'l', 'o')
  LINK(466, 462)
  CODE(467, _FLOAD)

// wag (  - )
  NAME(468, 0, 3, 'w', 'a', 'g')
  LINK(469, 465)
  CODE(470, _WAGDS)

// wiggle ( n  - )
  NAME(471, 0, 6, 'w', 'i', 'g')
  LINK(472, 468)
  CODE(473, _WIGGLE)

// rbyte ( adrs - n ) // consumes an address and returns a byte stored at that address
  NAME(474, 0, 5, 'r', 'b', 'y')
  LINK(475, 471) // patch that hole
  CODE(476, _RBYTE)

// compose (  - )
  NAME(477, 0, 2, 'c', 'c', 0) // named as the 'cc' word for now - was 'compose' too long to type blind
  LINK(478, 474)
  CODE(479, _COMPOSE)


  // test
  DATA(500, lit)
  DATA(501, 10) // i
  DATA(502, lit)
  DATA(503, 0) // i
  DATA(504, ddo)
  DATA(505, 400) // i
  DATA(506, dot)
  DATA(507, lloop)
  DATA(508, 505)
  DATA(509, 385) // R
  DATA(510, dot)
  DATA(511, ddots)
  DATA(512, cr)
  DATA(513, branch)
  DATA(514, 500)


  // D = 368; // latest word // D = 259;
  // H = 371; // top of dictionary // H = 262;

  // D = 471; // latest word // D = 259;
  // H = 474; // top of dictionary // H = 262;

  // D = 474; // latest word // D = 259;
  // H = 477; // top of dictionary // H = 262;

  D = 477; // latest word // D = 259;
  H = 480; // top of dictionary // H = 262;

  // D = 480; // latest word // D = 259;
  // H = 483; // top of dictionary // H = 262;

//  I = 500; // test
  // SERIAL_LOCAL_C.begin (38400);
  // while (!SERIAL_LOCAL_C);
  // delay(100);
  // fl_setup();
  flash_setup(); // flash_ops.cpp
  I = abort; // instruction pointer = abort

   _color_black_bg(); _color_yellow_fg();
   delay(2000);
   SERIAL_LOCAL_C.print  ("\r myForth Arm Cortex   de wa1tnr  ItsyBitsyM4 25 AUG 2019 03:10z\r");

   SERIAL_LOCAL_C.print  ("      Sun Aug 25 03:10:50 UTC 2019 0.1.9 good-compiler-aa-ff-aa_exp\r");
   SERIAL_LOCAL_C.print  ("      ++rlist +cc +blist +mkdir +write_File +fload   shred: abn-591\r");
   SERIAL_LOCAL_C.print  ("      words: fload wlist warm\r");
   SERIAL_LOCAL_C.print  ("      TEF MEK Hk\r");
}

// the loop function runs over and over again forever
void loop() {
  W = memory.data [I++];
  memory.program [W] ();
//  delay (300);
}
