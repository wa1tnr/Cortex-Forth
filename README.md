Cortex-Forth
============

Mon Aug 12 01:06:11 UTC 2019

Cortex-Forth is a Forth in C for an Arm Arduino, in development.

Status
======

12 August 2019:

Branch 'good-compiler-aa' now holds a developed Forth that
can read in Forth source, from files stored on (vfat formatted)
QSPI flashROM (the usual CircuitPython method of file storage).

Adafruit ItsyBitsy M4 is the test case.

Some variants/branches use Serial and others, Serial1 (the UART).

If unexpected results occur, check that first. ;)

Sample Run
==========

TODO: The sample run has not updated in quite a while.

 mc $ microcom -p /dev/ttyACM0
connected to /dev/ttyACM0
Escape character: Ctrl-\
Type the escape character followed by c to get to the menu or q to quit
myForth Arm Cortex - release 0.1.6 (reversed-ee) de wa1tnr - use CTRL J and try the words word
empty  Ok
 Ok
 Ok
 Ok
5?
4?
3?
h. spa__ wor__ .s . qui_ 0< dep__ num___ ?du exe____ fin_ , ! @ ove_ swa_ dro_ dup wor_ par__ cr emi_ key exi_

 Ok
