.entry LOOP
	.entry                   		LENGTH
	.extern L3
	.extern W
			mcr      m1      
	prn #1
	prn #3
	
	prn #5
endmcr
MAIN: mov S1.1 ,W
add r2,STR
LOOP: jmp W
prn #-5
mcr     m5



		prn #1
 		prn #3



endmcr
sub r1, r4
inc K

jmp MAIN

TESTSTE: jmp L1(#-1,r6)

mov S1.2 ,r3
bne L3
END: stop1
STR: .string "abcdef"
sub r1, r4
STR2: .string "abcdef  asd
STR3: .string abcdeffsdfs " sdf

.extern MAIN

 mov r3 , r7
LENGTH: .data 6,-9  ,+15, 2  ,1

		LOOP:   	jmp t		

K: .data 22
m1

jsr LOOP(#1,#5)

m5
m1


m5