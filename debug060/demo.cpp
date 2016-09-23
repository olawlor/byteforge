/**
  Boring demo program, demonstrating how to set variables
  and call functions.
*/
#include <iostream>
#include <string>
#include <map>
#include <stdlib.h> // for exit
#include <stdio.h>

// Random global variable; change this in gdb with:
//     set x=10
int x=3;

// Random global function; call this in gdb with:
//	print hero(123)
void hero(int arg) {
	for (int i=0;i<x;i++) std::cout<<"HELLO! "<<arg<<std::endl;
}

int main() {
	std::cout<<" x = "<<x<<"\n";
	return 0;
}


