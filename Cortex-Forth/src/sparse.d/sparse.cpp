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
extern int _COMPOSE(void);
extern void _KEY(void);

// extern char* parseStr(void);

char* parseStr(void) {
    _HERE();
    push(32);
    // push(' ');
    // _WORD();
    _ALLOT();
    _HERE();
    _SWAP();
    int n = pop(); // bottom address of new string allot'd
    char* cStr = (char*) n;
    char* str = 0;
    if(cStr) {
        *cStr++;
        _COMPOSE(); // _KEY();
        n = pop();
        cStr = (char*) n;
        str = (char*) cStr;
    }
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
