\ max ( n1 n2 -- max )
: max over over - 0< if swap drop -1 then if exit then swap drop ;

\ min ( n1 n2 -- min )
: min over over - 0< if drop exit then swap drop ;

\ >prn ( n -- 31<n<128 )
: >prn 32 max 127 min ;

\ alist ( addr -- )
: alist 16 + dup 16 - over over
  do 1 + over over swap - 0<
  if dup c@ >prn emit then loop
  drop ;

\ blist ( addr -- addr + CONST )
: blist depth 1 - 0< if 0 then
  111000 min 1 max 48 0 do alist loop ;
  \ need an accurate upper limit number there.
  \ 111000 is a stand-in and is meant to keep
  \ the user within the memory map limits
  \ of the c@ word

\ emits ( n1 n2 n3 .. nn count -- )
: emits 0 do emit loop ;

