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


mov S1.2 ,r3
bne L3
END: stop1
STR: .string "abcdef"
LENGTH: .data 6, -9  ,+15, 2  ,1 	

		LOOP:   	jmp t		

K: .data 22
m1


m5
m1


m5