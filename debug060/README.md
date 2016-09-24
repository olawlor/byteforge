Linux Command Line Debugging 060 
================================

This example will walk you through using the command-line debugger gdb.

First, run the demo program normally, using:

	./demo

Read the source code with:

	gedit demo.cpp &

To run this program in a debugger, use:

	gdb ./demo

We want to stop the program as soon as it starts up, so we tell gdb:

	b main

This sets a breakpoint at the function main.  Now you can (r) run the program, and it will immediately stop in main:

	r

You can now inspect the state of the program with commands like:

	print x

You can call functions or set variables as well:

	set x=10
	print hero(123)

You then continue running the program, and it will run normally from that point onward.

	c


Bank job
--------

Now, use your skills to become a VIP at the bank.  Run it with:

	./bank

Notice how if you owe more than $500, the bank sends GUIDO after you.
And you can't make deposits because the "teller" account is locked out.

Run in the debugger with:

	gdb ./bank

Challenges:
* Use the debugger to become an authorized teller and "deposit" 1000000 into your own account
* Use the debugger to initiate an offshore transfer and make yourself rich
* Use integer overflow to combine two loans into VIP status
* Read the disassembly to find the secret backdoor in the bank binary (git checkout bank if you already overwrote it!)


Disassembly and binary debugging
--------------------------------

Often you don't have source code (like with commercial applications and/or malware).  In these cases you need to debug based on a binary executable.

You can disassemble the program with objdump:

	objdump -drC -M intel ./bank > dis
	gedit dis &

You can extract long ASCII strings (literal strings, function names, etc) with "strings":

	strings ./bank > str
	gedit str &

You can dump the addresses of symbols (function names, global variables) with nm:

	nm ./bank > nm
	gedit nm &

If there are no symbols, because they've been stripped out, like:

	strip -o nobank bank

Without symbols you can't even set a breakpoint at main, but you can read the disassembly to see the start of the code, and set a breakpoint at a raw address with this gdb syntax:

	gdb ./demo
	b *0x400760
	r

You can even call functions from raw addresses, although you need to typecast the address to a function pointer first, using the awful C++ function pointer syntax:

	p ((void (*)(int))0x40089d)(123)

This does the same thing as "p hero(123)", but you don't need the symbol to do it.





