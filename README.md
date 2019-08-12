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

```
 $ microcom -p /dev/ttyACM0
connected to /dev/ttyACM0
Escape character: Ctrl-\
Type the escape character followed by c to get to the menu or q to quit
terminal - based on the Forth-like interpreter
  Ok
words [5 flo] [5 wli] [4 war] [4 typ] [2 c!] [2 c@] [135 lit] [134 rep]  
[133 whi] [133 aga] [1 '] [6 for] [132 els] [132 the] [130 if] [133 unt]  
[133 beg] [132 loo] [130 do] [1 i] [129 ;] [1 :] [1 ]] [129 []  
[1 R] [8 con] [1 ?] [8 var] [5 all] [4 her] [6 cre] [4 dum]  
[2 2/] [2 2*] [6 neg] [3 abs] [6 inv] [3 xor] [2 or] [3 and]  
[1 -] [1 +] [2 h.] [5 spa] [5 wor] [2 .s] [1 .] [7 flp]  
[4 qui] [2 0<] [5 dep] [6 num] [3 ?du] [7 exe] [4 fin] [1 ,]  
[1 !] [1 @] [4 ove] [4 swa] [4 dro] [3 dup] [4 wor] [5 par]  
[2 cr] [4 emi] [3 key] [4 exi]  Ok
-99  Ok
.s -99  Ok
fload  'quit'   to exit the fload word.    (no longer a useful hint - quit isn't necessary; EOF is detected automagically)
 Ok
 Ok
  ABCDE   
 Ok
EOF 
 Ok
88 87 86 85 84 5 emits TUVWX Ok
: sillix 0 do emit loop ;  Ok
88 87 86 85 82 5 sillix RUVWX Ok
.s -99  Ok
words [6 sil] [5 emi] [5 flo] [5 wli] [4 war] [4 typ] [2 c!] [2 c@]  
[135 lit] [134 rep] [133 whi] [133 aga] [1 '] [6 for] [132 els] [132 the]  
[130 if] [133 unt] [133 beg] [132 loo] [130 do] [1 i] [129 ;] [1 :]  
[1 ]] [129 [] [1 R] [8 con] [1 ?] [8 var] [5 all] [4 her]  
[6 cre] [4 dum] [2 2/] [2 2*] [6 neg] [3 abs] [6 inv] [3 xor]  
[2 or] [3 and] [1 -] [1 +] [2 h.] [5 spa] [5 wor] [2 .s]  
[1 .] [7 flp] [4 qui] [2 0<] [5 dep] [6 num] [3 ?du] [7 exe]  
[4 fin] [1 ,] [1 !] [1 @] [4 ove] [4 swa] [4 dro] [3 dup]  
[4 wor] [5 par] [2 cr] [4 emi] [3 key] [4 exi]  Ok
wlist fload wlist warm type c! c@ literal repeat while again ' forget else then if until begin loop do i ; : ] [ R constant ? variable allot here create dump 2/ 2* negate abs invert xor or and - + h. space words .s . quit 0< depth number ?dup execute find , ! @ over swap drop dup word parse cr emit key exit  Ok

 - - -  a separate entity, the forth source file, test.fs:

 $ cat test.fs
: emits 0 do emit loop ;
69 68 67 66 65 5
space space emits space space cr
70 79 69 3 emits cr

```
END.
