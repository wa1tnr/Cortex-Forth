#include <Arduino.h>

/*
  8 // push n to top of data stack
  9 void push(int n) {
 10   _DUP();
 11   T = n;
 12   // see the definition of _HERE() for a similar technique to create a new stack element.
 13 }
*/

/* 767 void _bl(void) { dStack_push(' '); } */

extern void push(int n);
extern int pop(void);
extern void _SHOWTIB(void);
extern void _WORD(void);
extern void _ALLOT(void);
extern void _HERE(void);
extern void _SWAP(void);
extern void _DROP(void);
extern int _COMPOSE(void);
extern void _KEY(void);
extern void _DUP(void);
extern void _CSTORE (void);
extern void _CFETCH (void);

extern const char * myAlphaCcp;

#define BUFFLEN 128
char instring[BUFFLEN];
char tempstring[BUFFLEN];
int length = 1;



/*
  1 #include <stdio.h>
  2 #include <string.h>
  3 
  4 #define BUFFLEN 128
  5 #define CHOP_LN 5
  6 
  7 char instring[BUFFLEN];
  8 char tempstring[BUFFLEN];
  9 
 10 void init_cutter(void) {
 11     strcpy(instring, print_string);
 12 }
*/

// extern char* parseStr(void);


void fetchStrBackwards(void) { // ( adrs -- )
    int j = pop();
    int count = 0;
    for (int index = (j+1); index < (j + 32); index++) {
        push(index); count++;
        _CFETCH();
        _DUP(); // make a copy of the stored number
        int rr = pop(); // destroy that copy
        if (rr == 0) {
          _DROP();
          count--;
        }
    }
    push(count);
}

/*
      +0.2.0-a.0 +squote +fdir_planned ++rlist +cc +blist +mkdir +write_File +fload   shred: abn-705

      words: fload wlist warm

      TEF MEK Hn-f
fload  loading a forth program from flashROM ..
ABCDE  
/forth/ascii_xfer_a001.txt was closed - Cortex-Forth.ino LINE 369
s" Hello  11 .s 1013 11 cpmem .s 1013 11 536950436 dup rlist  
200136A4 : 51 41 53 44 46 4A 4B 4C 51 57 45 52 00 00 00 00   QASDFJKLQWER.... 
200136B4 : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 

use 536950436
*/
/*

did what was asked ;)

to wit:
     +0.2.0-a.0 +squote +fdir_planned ++rlist +cc +blist +mkdir +write_File +fload   shred: abn-705

      words: fload wlist warm

      TEF MEK Hn-f
fload  loading a forth program from flashROM ..
ABCDE  
/forth/ascii_xfer_a001.txt was closed - Cortex-Forth.ino LINE 369
s" Hello_Kitty  .s 1013 10 cpmem .s empty 536950436 rlist  
200136A4 : B9 04 F5 80 74 44 45 0E D0 15 45 52 00 00 00 00   ....tDE...ER.... 
200136B4 : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
 
1013 rlist  
3F5 : B9 04 F5 80 74 44 45 0E D0 15 F8 01 3F 00 2B F7   ....tDE.....?.+. 
405 : D0 21 46 0B EB 04 00 4F F4 80 72 04 F5 80 74 01   .!F....O..r...t. 

*/

// this is not knowledge, it's wild guesswork:
void cpMem2Str(void) { // ( addr ln -- )
    instring[0] = 'Q';
    instring[1] = 'A';
    instring[2] = 'S';
    instring[3] = 'D';
    instring[4] = 'F';
    instring[5] = 'J';
    instring[6] = 'K';
    instring[7] = 'L';
    instring[8] = 'Q';
    instring[9] = 'W';
    instring[10] = 'E';
    instring[11] = 'R';
    instring[12] = ' ';
    instring[13] = 'J';
    instring[14] = 'E';
    instring[15] = 'L';
    instring[16] = 'L';
    instring[17] = 'Y';
    instring[18] = '-';
    instring[19] = 'R';
    instring[20] = 'O';
    instring[21] = 'L';
    instring[22] = 'L';
    instring[23] = '~';
    instring[24] = '\0';


    length = pop();
    // char* memAdrs = (char *) pop();
    int adrs = pop();
    const void* cvp = (const void*) adrs;
//  char* address = (char*) adrs;
//  char* memAdrs = address;
//  memcpy(instring, &memAdrs, length);

#ifdef WAS_LATEST_STRING_THING
    memcpy(instring, cvp, length);
#else
    // memcpy(instring, myAlphaCcp, 22);
    memcpy(instring, myAlphaCcp, length);
Serial.println(instring);
#endif
    push((int)&instring); // notha wileguess
}
/*
     TEF MEK Hn-f
fload  loading a forth program from flashROM ..
ABCDE  
/forth/ascii_xfer_a001.txt was closed - Cortex-Forth.ino LINE 369
s" drseus  .s 1013 77 cpmem abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_____THIS_IS_A_CONST_CHAR
.s 536950452 rlist  
200136B4 : 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F 70   abcdefghijklmnop 
200136C4 : 71 72 73 74 75 76 77 78 79 7A 41 42 43 44 45 46   qrstuvwxyzABCDEF 
200136D4 : 47 48 49 4A 4B 4C 4D 4E 4F 50 51 52 53 54 55 56   GHIJKLMNOPQRSTUV 
200136E4 : 57 58 59 5A 5F 5F 5F 5F 5F 54 48 49 53 5F 49 53   WXYZ_____THIS_IS 
200136F4 : 5F 41 5F 43 4F 4E 53 54 5F 43 48 41 52 00 00 00   _A_CONST_CHAR... 
20013704 : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
20013714 : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
20013724 : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
 
s" devia  .s 536950580 1045 swap drop .s 1045 88 cpmem abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_____THIS_IS_A_CONST_CHAR_PTR_string
rlist  
200136B4 : 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F 70   abcdefghijklmnop 
200136C4 : 71 72 73 74 75 76 77 78 79 7A 41 42 43 44 45 46   qrstuvwxyzABCDEF 
200136D4 : 47 48 49 4A 4B 4C 4D 4E 4F 50 51 52 53 54 55 56   GHIJKLMNOPQRSTUV 
200136E4 : 57 58 59 5A 5F 5F 5F 5F 5F 54 48 49 53 5F 49 53   WXYZ_____THIS_IS 
200136F4 : 5F 41 5F 43 4F 4E 53 54 5F 43 48 41 52 5F 50 54   _A_CONST_CHAR_PT 
20013704 : 52 5F 73 74 72 69 6E 67 00 00 00 00 00 00 00 00   R_string........ 
20013714 : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
20013724 : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
 
*/

void fetchStr(void) { // ( adrs -- )
    int j = pop();
    int count = 0;
    for (int index = (j+1); index < (j + 32); index++) {
        push(index); count++;
        _CFETCH();
        _DUP(); // make a copy of the stored number
        int rr = pop(); // destroy that copy
        if (rr < 21) { _DROP(); push(46); }
        if (rr > 126) { _DROP(); push(46); }
        if (rr == 0) {
          _DROP();
          count--;
          push(count);
          return;
        }
    }
    // should not reach here:
    // push(count);
    push(-399);
    push(-499);
    push(-599);
}

/* 

unproven:



\ bfi input:  ( count address -- ? )
\ -99 3 500

proven:

: bsz 64 ; : bmk bsz 1 - ; cr  
: bfi swap 1 + bmk and swap ; cr  
: sxa here bsz allot here swap 1 + ; cr  
: txb bfi .s cr ; cr  

: ldelay 1024 0 do 1 delay loop cr ; cr  

subst above for:
: sam -99 sxa begin ldelay 43 emit .s again ; cr  

junk:
\ : sxa here 64 allot here swap 1 + ; cr
\ : bfi dup dup bsz - 1 + bmk and ; cr  
\ : sam sxa begin ldelay 43 emit drop again ;  


*/

void parseStr(void) {
    _HERE();
    push(32);
    _ALLOT();
    _HERE();
    _SWAP();
    int n = pop(); // bottom address of new string allot'd
    n++; // might want to skip that first byte haha

    /* note: no clue as to what has to stay and what can profitably be removed: */
    char* cStr = (char*) n;
    char* str = 0;
    if(cStr) {
        *cStr++;
        _COMPOSE(); // _KEY();
        // Serial.println("DEBUG intercept AA in parseStr() ");
        int ln = pop();
        ln--; // not ascii 32 delimiter
        int p = n;
        for (int i = ln; i>0; i--) {
            _DUP(); int test = pop();

            if (test == 32) {
                // Serial.println(" T is 32 ");
                test = pop();
            }

            push(p);
            _CSTORE();
            p++;
            // value address c!
        }
        push(p); _CSTORE();
    }
    push(n);
    Serial.print(" "); // subtle ending
    // return str;
}
/*

 249 extern void printStr(char*);
 250 extern void printHex(cell_t);
 251 extern void printHex(cell_t, uint8_t);
 254 extern void _throw(char*);
 255 extern void _cr();
 256 extern void _bl();
 261

 262 char* parseStr(void) {
 263         _bl(); _word(); cell_t n = dStack_pop();
 264         cell_t* cStr = (cell_t*) n;
 265         char* str = 0;
 266         if(cStr) {
 267                 n = *cStr++; str = (char*) cStr;
 268                 printStr("\r\n "), Serial.print(n), printStr("-byte \""), printStr(str), printStr("\"");
 269         }
 270         printStr(" ==> str "), printHex((cell_t) str);
 272         return str;
 273 }

*/

/* quick log sample session 30 aug 18:56 utc:

 myForth Arm Cortex   de wa1tnr  ItsyBitsyM4 30 AUG 2019 13:33z

      Fri Aug 30 13:33:11 UTC 2019 0.2.0-alpha.0 non-usart-30_aug-aa-

      +0.2.0-a.0 +squote +fdir_planned ++rlist +cc +blist +mkdir +write_File +fload   shred: abn-705

      words: fload wlist warm

      TEF MEK Hn-f
fload  loading a forth program from flashROM ..
ABCDE
/forth/ascii_xfer_a001.txt was closed - Cortex-Forth.ino LINE 369
.s empty ss ?
: sayit emits space ; .s empty 99 99 99 3 sayigs ?
99 33 99 3 sayit c!c cr
.s empty s" gubernatorial ENDstr. fs@ sayit gubernatorial .s empty cr
s" chenanagins ENDstr. .s 1038 dup fs@ .s 1038 115 110 105 103 97 110 97 110 101 104 99 11 cr
sayit chenanagins .s 1038 dup blist
40E : 2D 73 6E 69 67 61 6E 61 6E 65 68 63 00 00 00 00   -snigananehc....
41E : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
42E : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
43E : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
44E : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................
45E : 00 00 07 73 66 61 4D 00 00 00 75 45 00 00 07 00   ...sfaM...uE....
46E : 00 00 02 00 00 00 21 01 00 00 05 61 6C 6F 4D 00   ......!....aloM.
47E : 00 00 75 45 00 00 06 00 00 00 8C 00 00 00 28 00   ..uE..........(.

better log:

 myForth Arm Cortex   de wa1tnr  ItsyBitsyM4 30 AUG 2019 13:33z

      Fri Aug 30 13:33:11 UTC 2019 0.2.0-alpha.0 non-usart-30_aug-aa-

      +0.2.0-a.0 +squote +fdir_planned ++rlist +cc +blist +mkdir +write_File +fload   shred: abn-705

      words: fload wlist warm

      TEF MEK Hn-f
fload  loading a forth program from flashROM ..
ABCDE 
/forth/ascii_xfer_a001.txt was closed - Cortex-Forth.ino LINE 369
: sayit emits space space ; .s empty cr  
.s empty s" KingOfTheRoad ENDstr. .s 1007 dup cr  
fs@ sayit KingOfTheRoad  .s 1007 cr  
blist  
3EF : 0E 64 61 6F 52 65 68 54 66 4F 67 6E 69 4B 00 00   .daoRehTfOgniK.. 
3FF : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
40F : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
41F : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
42F : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
43F : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
44F : 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ................ 
45F : 00 07 73 66 61 4D 00 00 00 75 45 00 00 07 00 00   ..sfaM...uE..... 
 
*/
