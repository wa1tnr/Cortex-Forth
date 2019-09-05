
\ blink D13 using pnwrite and pnmode to setup the GPIO pin:
: testpmd 1 13 pnmode 1 13 pnwrite ldelay 0 13 pnwrite ldelay ;
: blinker 8 0 do testpmd loop space 43 emit 43 emit space ;


: compx bottom 384 + 32 + tlist 36 blist blist ; \ comment


: iter 1 + dup rbyte h. space ; \ comment
: testee bottom 511 + rdump cr iter ; \ comment


\ exercise the rlist and blist words
\ rlist relies on rbyte whereas
\ blist relies on c@

\ accidentally aligned - do not rely on this:
: testff bottom 464 + rlist 84 blist ; \ comment


0 [if]

 Ok
: testff bottom 464 + rlist 84 blist ;

.s
  empty  Ok
testff
       
200001D0 : 00 00 00 00 D1 42 00 00 04 65 78 69 14 00 00 00   .....B...exi....    
200001E0 : D5 42 00 00 03 6B 65 79 17 00 00 00 59 5A 00 00   .B...key....YZ..    
200001F0 : 04 65 6D 69 1A 00 00 00 7D 5A 00 00 02 63 72 00   .emi....}Z...cr.    
20000200 : 1D 00 00 00 89 49 00 00 05 70 61 72 20 00 00 00   .....I...par ...    
20000210 : 79 5C 00 00 04 77 6F 72 23 00 00 00 61 4B 00 00   y\...wor#...aK..    
20000220 : 03 64 75 70 26 00 00 00 15 43 00 00 04 64 72 6F   .dup&....C...dro    
20000230 : 29 00 00 00 F5 42 00 00 04 73 77 61 2C 00 00 00   )....B...swa,...    
20000240 : 69 43 00 00 04 6F 76 65 2F 00 00 00 95 43 00 00   iC...ove/....C..    
 
 
54 : 00 00 00 00 D1 42 00 00 04 65 78 69 14 00 00 00   .....B...exi....    
64 : D5 42 00 00 03 6B 65 79 17 00 00 00 59 5A 00 00   .B...key....YZ..    
74 : 04 65 6D 69 1A 00 00 00 7D 5A 00 00 02 63 72 00   .emi....}Z...cr.    
84 : 1D 00 00 00 89 49 00 00 05 70 61 72 20 00 00 00   .....I...par ...    
94 : 79 5C 00 00 04 77 6F 72 23 00 00 00 61 4B 00 00   y\...wor#...aK..    
A4 : 03 64 75 70 26 00 00 00 15 43 00 00 04 64 72 6F   .dup&....C...dro    
B4 : 29 00 00 00 F5 42 00 00 04 73 77 61 2C 00 00 00   )....B...swa,...    
C4 : 69 43 00 00 04 6F 76 65 2F 00 00 00 95 43 00 00   iC...ove/....C..    
 
 Ok

[then]


