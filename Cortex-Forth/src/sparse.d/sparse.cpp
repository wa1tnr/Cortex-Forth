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

// extern char* parseStr(void);

void fetchStr(void) { // ( adrs -- )
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

char* parseStr(void) {
    _HERE();
    push(32);
    _ALLOT();
    _HERE();
    _SWAP();
    int n = pop(); // bottom address of new string allot'd
    n++; // might want to skip that first byte haha

    char* cStr = (char*) n;
    char* str = 0;
    if(cStr) {
        *cStr++;
        _COMPOSE(); // _KEY();
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
    Serial.print("ENDstr. ");
    return str;
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
