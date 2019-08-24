#include <Arduino.h>

extern int T;
extern void _DROP(void);
extern void _DUP(void);
/* from Metro-M4-Express-interpreter/interpret_m4/interpret_m4.ino */

// push n to top of data stack
void push(int n) {
  _DUP(); // p = (p + 1)& STKMASK;
  T = n; // TOS = n;
  // see the definition of _HERE() for a similar technique to create a new stack element.
}

/*
 63 // push n to top of data stack
 64 void push(int n) {
 65   p = (p + 1)& STKMASK;
 66   TOS = n;
 67 }
*/

// return top of stack
int pop() {
  int n = T;
  _DROP(); // take care of stack pointer // p = (p - 1)& STKMASK;
  return n;
}


/*
 69 // return top of stack
 70 int pop() {
 71   int n = TOS;
 72   p = (p - 1)& STKMASK;
 73   return n;
 74 }
*/


// dump 16 bytes of RAM in hex with ascii on the side
void _dumpRAM(void) {
  char buffer[5] = "";
  char *ram;
  int p = pop();
  ram = (char*)p;
  sprintf(buffer, "%4x", p);
  Serial.print(buffer);
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    char c = *ram++;
    sprintf(buffer, " %2x", (c & 0xff));
    Serial.print(buffer);
  }
  ram = (char*)p;
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    buffer[0] = *ram++;
    if (buffer[0] > 0x7f || buffer[0] < ' ') buffer[0] = '.';
    buffer[1] = '\0';
    Serial.print(buffer);
  }
  push(p + 16);
}


/*
273 // dump 16 bytes of RAM in hex with ascii on the side
274 void dumpRAM() {
275   char buffer[5] = "";
276   char *ram;
277   int p = pop();
278   ram = (char*)p;
279   sprintf(buffer, "%4x", p);
280   Serial.print(buffer);
281   Serial.print("   ");
282   for (int i = 0; i < 16; i++) {
283     char c = *ram++;
284     sprintf(buffer, " %2x", (c & 0xff));
285     Serial.print(buffer);
286   }
287   ram = (char*)p;
288   Serial.print("   ");
289   for (int i = 0; i < 16; i++) {
290     buffer[0] = *ram++;
291     if (buffer[0] > 0x7f || buffer[0] < ' ') buffer[0] = '.';
292     buffer[1] = '\0';
293     Serial.print(buffer);
294   }
295   push(p + 16);
296 }
*/

void _getOneByteRAM(void) { // ( addr -- )
  char *ram;
  int p = pop(); // address to investigate
  ram = (char*)p;
  char c = *ram++;
  push((int) c); // can we do this?
}

