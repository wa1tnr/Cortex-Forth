: 0= dup abs negate 0< invert swap drop ;
: max over over - 0< if swap drop -1 then if exit then swap drop ;
: min over over - 0< if drop exit then swap drop ;
: testcc -1 512 0 do 1 + dup , loop ;
  
: >prn 32 over over - 
  0< if 
      46 emit drop drop exit 
  then 
  drop 127 1 - over over swap - 
  0< if 
      46 emit drop drop exit 
  then 
  drop emit ;
: delay drop 1234 0 do 1 drop loop ;
: ecol 58 emit ;
: hadr dup 1 + h. ecol space ;
  
: rhlist hadr 16 + dup 16 - over over 
  do 
      1 + over over swap - 1 - 
      0< if 
          dup rbyte dup 16 - 
          0< if 
              48 emit 
          then 
          h. 100 delay 
      then 
  loop 
  drop ;
  
  
  
: ralist 
  space space 16 + dup 16 - over over 
  do 
      1 + over over swap - 1 - 
      0< if 
          dup rbyte >prn 100 delay 
      then 
  loop drop ;
: hlist hadr 16 + dup 16 - over over do 1 + over over swap - 1 - 0< if dup c@ dup 16 - 0< if 48 emit then h. 100 delay then loop drop ;
: alist space space 16 + dup 16 - over over do 1 + over over swap - 1 - 0< if dup c@ >prn 100 delay then loop drop ;
: bottom 536870912 ;
: topbottom bottom 16384 + 1024 - 1024 + 16 - ;
: blist cr -999 swap 196604 1148 - min 0 max dup 1 - 8 0 do dup hlist 16 - alist cr swap drop loop 1 + swap drop cr ;
: rlist cr -999 swap bottom 195552 + min 0 max dup 1 - 8 0 do dup rhlist 16 - ralist cr swap drop loop 1 + swap drop cr ;
: emits 0 do emit loop space ;
: said fs@ emits space cr space ;
: stuffit 69 68 67 66 65 5 ; 69 68 67 66 65 5 emits cr 2048 allot variable bend variable buff here buff ! variable bend variable buff here buff ! 2048 allot here bend ! 1 drop : svd buff @ 2701 + blist ;
: sve buff @ 4 + cr ;
: goa svd sve 26 0 do 1 + 32 i + over ! loop cr cr svd cr ;
variable sfi 0 sfi ! 1 drop variable kbi -1 kbi ! variable kst 254 kst ! 1 drop : bsz 128 ; : bmk bsz 1 - ;
: wep 32 111 116 32 121 97 119 32 116 115 101 105 115 97 101 32 101 104 84 19 emits ;
: sbl dup blist drop ;
: tellme drop ;
: sxt 1 - bmk and ;
: sxo bmk and bmk 2 - ;
: sxk sxo over over ;
: sxf swap 1 + sxk swap - ;
: k-- swap 1 - sxk swap - 0< if swap sxt sxt sxt swap then drop dup 0= if 1 - sxo then swap ;
: sxc 1 + bmk and ;
: sxd 3 0 do sxc loop ;
: sxe 0< if swap sxd swap then ;
: sxh       0= if 
                k-- k-- 
            then 
            sxf sxe ;
: k++ kst @ 199 - sxh drop swap ;
: p-- space space k-- space space .s space cr ;
: p++ space space k++ space space .s space cr ;
: mhe 72 emit 101 emit 114 emit 101 emit 58 emit space ;
: bfc 0 ;
: sxa mhe here dup . bsz allot here swap 1 + ;
: lxa -99 sxa ;
: ctrl! dup 31 -   0< if       31 kst !   then   67 tellme ;
: bksp!   dup 8 - 0= if       199 kst !   then   80 tellme ;
: escp! dup 27 - 0= if       227 kst !   then   84 tellme ;
: sesc       95 67 83 69 95       83 84 73 95 89       69 72 12   emits ;
: hes kst @ 227 -   0= if       sesc drop quit   then ;
: hcp drop drop swap 1 - swap ;
: hco kst @ 31 -   0= if       kbi @ 1 - kbi !       hcp sbl   then ;
: hbk kst @ 199 - 0= if 1 drop then ;
: sxp kbi @ 1 + kbi ! 254 kst ! 1 drop key ctrl! bksp! escp! hes hbk kst @ 254 - 0= if swap c! then kst @ 199 - 0= if drop drop then 77 tellme hco k++ over over + 1 drop ;
: sxb begin sxp again ;
: sam lxa bfc swap k++ over over + sxb ;
