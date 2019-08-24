#include <Arduino.h>

extern int T;
extern void _DROP(void);
extern void _DUP(void);
/* from Metro-M4-Express-interpreter/interpret_m4/interpret_m4.ino */

// push n to top of data stack
void push(int n) {
  _DUP();
  T = n;
  // see the definition of _HERE() for a similar technique to create a new stack element.
}

// return top of stack
int pop() {
  int n = T;
  _DROP(); // take care of stack pointer
  return n;
}

// dump 16 bytes of RAM in hex with ascii on the side
void _dumpRAM(void) {
}

void _getOneByteRAM(void) { // ( addr -- )
  char *ram;
  int p = pop(); // address to investigate
  ram = (char*)p;
  char c = *ram++;
  push((int) c); // can we do this?
}

