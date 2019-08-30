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
extern void _DUP(void);
extern void _CSTORE (void);

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
    n++; // might want to skip that first byte haha

    // ( here -- here 32 -- here -- here heretop -- heretop here -- heretop )

    char* cStr = (char*) n;
    char* str = 0;
    if(cStr) {
        *cStr++;
        _COMPOSE(); // _KEY();
        Serial.print(" n in parseStr is now: "); Serial.println(n);
        // _DUP();
        // n = pop(); // bottom address of string buffer?
        // cStr = (char*) n; // our new string, empty tho
        // strcpy (str, cStr);
        // Serial.print((int)str, HEX); Serial.println(" wonder what that was");
        // Serial.print(str, HEX); Serial.println(" wonder what that was");
        int p = n;
        for (int i = 32; i>0; i--) {
            Serial.print("loop index: "); Serial.print(i); Serial.print("  ");
            push(p);
            _CSTORE();
            Serial.print("p becomes: "); Serial.print(p); Serial.print("  ");
            p++;
            // value address c!


            // strcpy(cStr, (char*) pop());
            // strcpy(str++, (char*) pop());
        }
        push(p); _CSTORE();
        // str = (char*) cStr;
    }
    Serial.println("Returning str in parseStr: ");
    Serial.print("final n is: ");
    Serial.println(n);
    push(n);
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
