.entry LOOP
	.entry LENGTH
	.extern L3
	.extern W
MAIN: mov S1.1 ,W
add r2,STR
LOOP: jmp W
prn #-5
K: .data 22
S1: .struct 8, "ab"


m1
