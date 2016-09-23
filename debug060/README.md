Linux Command Line Debugging 060 
================================

This example will walk you through using the command-line debugger gdb.

First, run the demo program normally, using:

	./demo

Read the source code with:

	gedit demo.cpp

To run this program in a debugger, use:

	gdb ./demo

We want to stop the program as soon as it starts up, so we run:

	b main

This sets a breakpoint at the function main.  Now you can run the program, and it will immediately stop in main:

	r

You can now inspect the state of the program with commands like:

	print x

You can call functions or set variables as well:

	set x=10
	print hero(123)

You then continue running the program, and it will run from that point onward.

	c


Now, use your skills to become a VIP at the bank.

Notice how if you owe more than $500, the bank sends GUIDO after you.
And you can't make deposits because the "teller" account is locked out.

Challenges:
* Use the debugger to become a teller and "deposit" 1000000 into your own account
* Use the debugger to initiate an offshore transfer and make yourself rich
* Use integer overflow to combine two loans into VIP status
* Read the disassembly to find the secret backdoor in the bank binary (git checkout bank if you already overwrote it!)


