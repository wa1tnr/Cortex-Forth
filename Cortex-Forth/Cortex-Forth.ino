// Mon Aug 12 02:33:44 UTC 2019 0.1.8 good-compiler-aa  shred: abn-493

// On branch  good-compiler-aa

// identify: kruvulax  bunaka  vimaxl  helmkuttr  chowfex  tubabr

// target: ItsyBitsy M4 Express - still current on branch  good-compiler-aa  12 August 2019
// comm: USB, not the TX/RX pair for the Forth interpreter - on branch  good-compiler-aa
// terminal: minicom (provides keystroke echo)
// the other method is to construct a 'terminal' from a Trinket M0 and use the UART ;)

// Note: other branches may want to use the UART rather than USB.

/*
  Forth virtual machine

  This code is in the public domain.

*/
#include <SdFat.h> // 'File' 10 Aug 16:10z

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
  Serial.println(" loading a forth program from flashROM ..");
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
 126   Serial.write (c);
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
  while (!Serial.available ());
  T = Serial.read ();
//  Serial.write (T);
}

void _EMIT (void) {
  char c = T;
  Serial.write (c);
  _DROP ();
}

void _CR (void) {
  Serial.println (" ");
}

void _OK (void) {
  if (tib [tib.length () - 1] == LINE_ENDING) Serial.println (" Ok");
}

void _WLIST (void) {
  Serial.print ("wiggle wag fload wlist warm type c! c@ literal repeat while again ' forget else then if until begin loop do i ; : ] [ R constant ? variable allot here create dump 2/ 2* negate abs invert xor or and - + h. space words .s . flparse quit 0< depth number ?dup execute find , ! @ over swap drop dup word parse cr emit key exit ");
}

void _WARM (void) {
  NVIC_SystemReset();      // processor software reset
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
  Serial.print (tib); // tnr // restored to original
}

// trim leading spaces
void _PARSE (void) {
  char t;
  tib = "";
  do {
    while (!Serial.available ());
    t = Serial.peek ();
    if (t == ' ') {
      t = Serial.read ();
//      Serial.write (t);
    }
  } while (t == ' ');
  do {
    while (!Serial.available ());
    t = Serial.read ();

#ifdef ECHO_INPUT
    Serial.write (t);
#endif

    tib = tib + t;
  } while (t > ' ');
  // tnr, suppressed // Serial.print (tib);
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
    // Serial.println("DEBUG 12 Aug - thisFile does exist - GOOD."); // tnr 12 Aug kludge
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
      // Serial.print("  _"); Serial.print(tib); Serial.print("_  ");
      if (thisFile.available() < (FLEN_MAX - 1)) {
        // Serial.println("\n\n\nSAFETY NET\n\n\n");
        if (thisFile.available() < (1)) {
          keyboard_not_file = true;
          thisFile.close(); // experiment 17:06z 11 Aug
          Serial.print("\r");
          Serial.print(FILE_NAME);
          Serial.println(" was closed - Cortex-Forth.ino LINE 347");
/*
          _DDOTS(); // experiment 16:48z 11 Aug
          _SPACE();
          _SPACE();
          Serial.print("xxx");
          _SPACE();
          _SPACE();
          _SPACE();
          _SPACE();
          _SPACE();
          Serial.print("yyy");
          _SPACE();
          _SPACE();
          _SPACE();
          _DEPTH();
          _DDOTS();
          _SPACE();
          _SPACE();
          Serial.println("\n previous line: _DDOTS();");
          delay(100);
          // while(-1); // permanent trap 11 Aug 16:45 UTC 2019
*/
        }
      }
      // Serial.println("TRAP");
      return; // EXPERIMENT - this could crash it - not sure why but the TRAP lines are ignored in Forth - but the very last line was not ignored and made it onto the stack (it was a pushed value).
    } // new conditional 17:25z
    Serial.println(" alt TRAP LINE 334");
    delay(1400); // KLUDGE tnr kludge 12 Aug 23:15
  } // if thisfile
  else {
    // Serial.print("Trouble at the Old Well, Timmy?");
    // Serial.print(" I = 90 -- the 'parse' word  ");
    // Serial.println(" alt TRAP LINE 339");
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
  // Serial.print(" ~k~ ");
  // Serial.println(" --- _WORD  exits --- ");
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
      // Serial.println("FIND exits - and its a word.");
      return;
    }
    T = memory.data [T + 1];
  }
  // Serial.println("FIND exits.");
}

void _DOT (void) {
  Serial.print (T);
  Serial.write (' ');
  _DROP ();
}

void _HDOT (void) {
  Serial.print (T, HEX);
  Serial.write (' ');
  _DROP ();
}

void _DDOTS (void) {
  if (S == S0) {
    Serial.print ("empty ");
    return;
  }
  _DUP ();
  W = (S0 - 1);
  while (W > (S)) {
    Serial.print (memory.data [--W]);
    Serial.write (' ');
  }
  _DROP ();
}

void _SPACE () {
  Serial.write (' ');
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
  Serial.write ('[');
  Serial.print (X);
  Serial.write (' ');
  X = ((Y >> 8) & 0xff);
  _DUP (); T = X; _EMIT ();
  X = ((Y >> 16) & 0xff);
  if (X != 0) { _DUP (); T = X; _EMIT (); }
  X = ((Y >> 24) & 0xff);
  if (X != 0) { _DUP (); T = X; _EMIT (); }
  Serial.print ("] "); 
}

void _WORDS (void) {
  int i = 0;
  W = D;
  do {
    _DOTWORD ();
    W = memory.data [++W];
    i += 1;
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
    Serial.print (memory.data [T++], HEX);
    // Serial.write (' ');
    Serial.write (" ~dump_delimiter~ ");
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
  Serial.println("WHOOPS - _TICK encountered! ");
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
  Serial.print("\033\133"); // ESC [
  Serial.print("\063\063"); // 33 - yellow fg
  Serial.print("m");        // for the stanza
}

void _color_blue_bg (void) {
  Serial.print("\033\133"); // ESC [
  Serial.print("\064\064"); // 44 - blue bg
  Serial.print("m");        // for the stanza
}

void _color_black_bg (void) {
  Serial.print("\033\133"); // ESC [
  Serial.print("\064\060"); // 40 - black bg
  Serial.print("m");        // for the stanza
}


void setup () {
#ifdef HAS_DOTSTAR_LIB
  setup_dotstar(); // turn off dotstar (apa-102 RGB LED)
  // set_dotStarColors(); // give them some color
  // wiggleDSManyTimes(); // repeat the toggle
#endif // #ifdef HAS_DOTSTAR_LIB

  // Serial.begin (38400); while (!Serial);

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


  // . ( n - )
  NAME(217, 0, 1, '.', 0, 0)
  LINK(218, 135)  // 86 // if this isn't 86 then the quit word is lost -- old: NEAT - links back to the quit word here
  CODE(219, _DOT)
#  define dot 219
  // .s
  NAME(220, 0, 2, '.', 's', 0)
  LINK(221, 217)
  CODE(222, _DDOTS)
#  define ddots 222
  // words
  NAME(223, 0, 5, 'w', 'o', 'r')
  LINK(224, 220)
  CODE(225, _WORDS)
#  define words 225
  // space
  NAME(226, 0, 5, 's', 'p', 'a')
  LINK(227, 223)
  CODE(228, _SPACE)
#  define space 228
  // h. ( n - )
  NAME(229, 0, 2, 'h', '.', 0)
  LINK(230, 226)
  CODE(231, _HDOT)
#  define hdot 231
  // + ( n1 n2 - n3)
  NAME(232, 0, 1, '+', 0, 0)
  LINK(233, 229)
  CODE(234, _PLUS)
#  define plus 234
  // - ( n1 n2 - n3)
  NAME(235, 0, 1, '-', 0, 0)
  LINK(236, 232)
  CODE(237, _MINUS)
  // and (n1 n2 - n3)
  NAME(238, 0, 3, 'a', 'n', 'd')
  LINK(239, 235)
  CODE(240, _aND)
  // or ( n1 n2 - n3)
  NAME(241, 0, 2, 'o', 'r', 0)
  LINK(242, 238)
  CODE(243, _OR)
  // xor ( n1 n2 - n3)
  NAME(244, 0, 3, 'x', 'o', 'r')
  LINK(245, 241)
  CODE(246, _XOR)
  // invert ( n1 - n2)
  NAME(247, 0, 6, 'i', 'n', 'v')
  LINK(248, 244)
  CODE(249, _INVERT)
  // abs ( n1 - n2)
  NAME(250, 0, 3, 'a', 'b', 's')
  LINK(251, 247)
  CODE(252, _ABS)
  // negate ( n1 - n2)
  NAME(253, 0, 6, 'n', 'e', 'g')
  LINK(254, 250)
  CODE(255, _NEGATE)
  // 2* ( n1 - n2)
  NAME(256, 0, 2, '2', '*', 0)
  LINK(257, 253)
  CODE(258, _TWOSTAR)
  // 2/ ( n1 - n2)
  NAME(259, 0, 2, '2', '/', 0)
  LINK(260, 256)
  CODE(261, _TWOSLASH)
  // dump ( a n - a+n) 
  NAME(262, 0, 4, 'd', 'u', 'm')
  LINK(263, 259)
  CODE(264, _DUMP)
  // create
  NAME(265, 0, 6, 'c', 'r', 'e')
  LINK(266, 262)
  CODE(267, _CREATE)
  // here 
  NAME(268, 0, 4, 'h', 'e', 'r')
  LINK(269, 265)
  CODE(270, _HERE)
  // allot 
  NAME(271, 0, 5, 'a', 'l', 'l')
  LINK(272, 268)
  CODE(273, _ALLOT)
  // variable 
  NAME(274, 0, 8, 'v', 'a', 'r')
  LINK(275, 271)
  CODE(276, _VARIABLE)
  // ?
  NAME(277, 0, 1, '?', 0, 0)
  LINK(278, 274)
  CODE(279, _QUESTION)
  // constant
  NAME(280, 0, 8, 'c', 'o', 'n')
  LINK(281, 277)
  CODE(282, _CONSTANT)
  // R
  NAME(283, 0, 1, 'R', 0, 0)
  LINK(284, 280)
  CODE(285, _R)
  // [ 
  NAME(286, IMMED, 1, '[', 0, 0)
  LINK(287, 283)
  CODE(288, _LBRAC)
  // ]
  NAME(289, 0, 1, ']', 0, 0)
  LINK(290, 286)
  CODE(291, _RBRAC)
  // :
  NAME(292, 0, 1, ':', 0, 0)
  LINK(293, 289)
  CODE(294, _COLON)
  // ;
  NAME(295, IMMED, 1, ';', 0, 0)
  LINK(296, 292)
  CODE(297, _SEMI)
  // i 
  NAME(298, 0, 1, 'i', 0, 0)
  LINK(299, 295)
  CODE(300, _I)
#  define _i 300
  // do
  NAME(301, IMMED, 2, 'd', 'o', 0)
  LINK(302, 298)
  CODE(303, _CDO)
  // loop 
  NAME(304, IMMED, 4, 'l', 'o', 'o')
  LINK(305, 301)
  CODE(306, _CLOOP)
  // begin 
  NAME(307, IMMED, 5, 'b', 'e', 'g')
  LINK(308, 304)
  CODE(309, _CBEGIN)
  // until 
  NAME(310, IMMED, 5, 'u', 'n', 't')
  LINK(311, 307)
  CODE(312, _CUNTIL)
  // if
  NAME(313, IMMED, 2, 'i', 'f', 0)
  LINK(314, 310)
  CODE(315, _CIF)
  // then
  NAME(316, IMMED, 4, 't', 'h', 'e')
  LINK(317, 313)
  CODE(318, _CTHEN)
  // else
  NAME(319, IMMED, 4, 'e', 'l', 's')
  LINK(320, 316)
  CODE(321, _CELSE)
  // forget
  NAME(322, 0, 6, 'f', 'o', 'r')
  LINK(323, 319)
  CODE(324, _FORGET)
  // '
  NAME(325, 0, 1, '\'', 0, 0)
  LINK(326, 322)
  CODE(327, _TICK)
  // again
  NAME(328, IMMED, 5, 'a', 'g', 'a')
  LINK(329, 325)
  CODE(330, _CAGAIN)
  // while
  NAME(331, IMMED, 5, 'w', 'h', 'i')
  LINK(332, 328)
  CODE(333, _CWHILE)
  // repeat
  NAME(334, IMMED, 6, 'r', 'e', 'p')
  LINK(335, 331)
  CODE(336, _CREPEAT)
  // literal 
  NAME(337, IMMED, 7, 'l', 'i', 't')
  LINK(338, 334)
  CODE(339, _CLITERAL)
  // c@ ( b - c) 
  NAME(340, 0, 2, 'c', '@', 0)
  LINK(341, 337)
  CODE(342, _CFETCH)
#  define cfetch 342
  // c! ( c b - ) 
  NAME(343, 0, 2, 'c', '!', 0)
  LINK(344, 340)
  CODE(345, _CSTORE)
  // type ( b c - ) 
  NAME(346, 0, 4, 't', 'y', 'p')
  LINK(347, 343)
  CODE(348, _NEST)
  DATA(349, over)
  DATA(350, plus)
  DATA(351, swap)
  DATA(352, ddo)
  DATA(353, _i)
  DATA(354, cfetch)
  DATA(355, emit)
  DATA(356, lloop)
  DATA(357, 353)
  DATA(358, exit)
  // warm (  - )
  NAME(359, 0, 4, 'w', 'a', 'r')
  LINK(360, 346)
  CODE(361, _WARM)

  // wlist (  - )
  NAME(362, 0, 5, 'w', 'l', 'i')
  LINK(363, 359)
  CODE(364, _WLIST)

  // fload (  - )
  NAME(365, 0, 5, 'f', 'l', 'o')
  LINK(366, 362)
  CODE(367, _FLOAD)

  NAME(368, 0, 3, 'w', 'a', 'g')
  LINK(369, 365)
  CODE(370, _WAGDS)

  NAME(371, 0, 6, 'w', 'i', 'g')
  LINK(372, 368)
  CODE(373, _WIGGLE)



  // test
  DATA(400, lit)
  DATA(401, 10) // i
  DATA(402, lit)
  DATA(403, 0) // i
  DATA(404, ddo)
  DATA(405, 300) // i
  DATA(406, dot)
  DATA(407, lloop)
  DATA(408, 405)
  DATA(409, 285) // R
  DATA(410, dot)
  DATA(411, ddots)
  DATA(412, cr)
  DATA(413, branch)
  DATA(414, 400)


  // D = 368; // latest word // D = 259;
  D = 371; // latest word // D = 259;
  // H = 371; // top of dictionary // H = 262;
  H = 374; // top of dictionary // H = 262;

//  I = 400; // test
  // Serial.begin (38400);
  // while (!Serial);
  // delay(100);
  // fl_setup();
  flash_setup(); // flash_ops.cpp
  I = abort; // instruction pointer = abort

  _color_black_bg(); _color_yellow_fg();
  delay(2000);
// Mon Aug 12 02:33:44 UTC 2019 0.1.8 good-compiler-aa  shred: abn-493
  Serial.println  ("\n myForth Arm Cortex   de wa1tnr  ItsyBitsyM4 12 AUG 2019 02:33z");
// Serial.println ("\n      Mon Aug 12 00:26:02 UTC 2019 0.1.8 side-fload-bb-ee-aa-aa");
  Serial.println  ("\n      Mon Aug 12 02:33:44 UTC 2019 0.1.8 good-compiler-aa");
  Serial.println  ("\n      +fload - clean compile.  shred: abn-493 ");
  Serial.println  ("\n      words: fload wlist warm");
}

// the loop function runs over and over again forever
void loop() {
  W = memory.data [I++];
  memory.program [W] ();
//  delay (300);
}
