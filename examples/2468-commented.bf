This brainfuck program prints the following 
suits on the screen : 2468

++++		the number of number to print
[		
	>	Move to the accumulator cell
	++	Add two to the accumulator 
		which leads to the multiplication
		of the loop counter
	.	Print the accumulator
	<-	move back to the loop counter and 
		decrement it
]
