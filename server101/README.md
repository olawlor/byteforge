Linux Server Security 101
=========================

This example will walk you through some simple server setup, and reverse engineering problems.

Starting from zero, you should:

1. Boot a linux distribution, including at least g++ git make

2. Open a terminal, by pressing ctrl-alt-T (for Terminal)
		
3. Clone this repository, with:

		git clone https://github.com/olawlor/byteforge

4. Change to this directory, with:

		cd byteforge/server101

5. Edit this file, with:

		gedit README.md



Eat some small fish
-------------------

See what services are running on your machine with:

	netstat -plunt

Run this service with:

	./service

(It will keep waiting in the foreground.  Press ctrl-C to kill it.  Open a second terminal with ctrl-alt-T to run other commands.)

Check netstat again to verify your server is running, and see what port it's listening on.

Connect to your new server using netcat (nc) and the loopback IP address 127.0.0.1.  Type your name after the server sends its start string:

	nc 127.0.0.1 1234

Uh oh, it looks like your server is not OK!

Check the source code, server.cpp, to see why.  Ah, there's a global variable "working_OK" that is currently set to zero!  Find and fix it by first finding your server's process ID (PID):

	ps | grep service

Now connect the debugger gdb to this PID with:

	sudo gdb -p <PID from above>

gdb has a ton of neat commands, like:

* **p** (Print) shows the value of a variable, like working_OK.  You can even do expressions, like &working_OK to get its address, or typecasts like *(int *)0x54321 to examine memory, or even variable assignments.
* **where** shows you where gdb has stopped the program.
* **b functionName** (Break) will make gdb stop when this function runs.
* **c** (Continue) lets the program keep running.
* **q** (Quit) leaves gdb, so you can test the server again.

Fix the service so it's OK!  The changes you make with gdb only apply while that copy of the server is running.


Eat big fish
------------

You can extract a lot of information from a compiled program, like this service, by using disassembly tools to examine the program itself:

	strings service > str.txt
	nm service > sym.txt
	ldd service > libs.txt
	objdump -M intel -drC service > asm.txt

Each of these commands makes a text file (redirected from the screen with ">").  Use gedit to read the text file--note how long they can get!  You can use any unix tool to search or compare in the files:

	grep OK sym.txt
	diff sym.txt old_sym.txt

This particular program is vulnerable to a buffer overflow attack:

  http://insecure.org/stf/smashstack.html

By echoing a carefully crafted string to netcat, you can overwrite the function return address in old_say_hello, and take control of the server.  An easy way to demonstrate this is to write in the address of the function "target"; see sploit.cpp for details:

	grep target sym.txt
	make sploit
	./sploit 0x080492d4 | nc 127.0.0.1 1234

If you're trying to do anything complex with this sort of attack, you need to do a lot of reading disassembly and assembly language debugging.  gdb is very useful for this low-level work (http://imgur.com/9f5Cuef):

* **i r** (Info about Registers) shows the current values of the assembly language registers.
* **b *0x54321** (Breakpoint) at an address.  It's often handy to stop at the instruction right before we do the fatal return instruction.
* **disassemble 0x54321** treats an address as executable code, and disassembles it.
* **x/64x $rsp** dumps memory in hex (x) around the stack pointer $rsp.


Note that modern machines include a number of protections against this sort of 
attack, so the above example won't work, but more advanced attacks can still succeed.

  http://www.madhur.co.in/blog/2011/08/06/protbufferoverflow.html

  https://www.blackhat.com/presentations/bh-europe-09/Fritsch/Blackhat-Europe-2009-Fritsch-Buffer-Overflows-Linux-whitepaper.pdf


Learn to fish
-------------

Learn more about using netstat, or any command above, with:

	man netstat

Learn more about a running process by examining the files in /proc/<PID>

Learn more about assembly language at Dr. Lawlor's CS 301 page:

  https://www.cs.uaf.edu/courses/cs301/2014-fall/



Remember: with great power, comes great responsibility.
By Dr. Orion Lawlor, lawlor@alaska.edu, 2015-09-25 (Public Domain)

