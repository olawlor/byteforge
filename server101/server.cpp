/**
  Simple network service, demonstrating security vulnerabilities.
*/
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h> // for fork and such
#include "osl/socket.h"
#include "osl/socket.cpp" // <- to make linking easier

/* This target function demonstrates the capabilities of buffer overflows */
void target(void) {
	std::cout<<"Whoop, whoop!  You got here!"<<std::endl;
	std::cout<<"  ... now to crash!"<<std::endl;
}

/* Old school C style code */
void old_say_hello(const char *str) {
	// copy weird C++ str into plain C dest buf
	char buf[128]; // this many chars should be enough for anyone.
	char *dest=&buf[0];
	while (*str!=0) 
		*dest++ = *str++;
	*dest=0; // add nul terminator
	
	printf("Hello, %s\n",buf); 
}

std::string get_address(void *ptr) {
	char buf[64];
	snprintf(buf,sizeof(buf),"0x%08lx",(long)ptr);
	return buf;
}


// Status globals:
int working_OK=0;
int is_hackable=1;
std::string magic="none";

// Main network access function:
void handle_client(SOCKET s, bool intranet) {
	std::string status="Server ";
	status+=get_address(&status);
	status+=": ";
	if (working_OK) status+="TOTALLY OK";
	else status+="not really ok";

	status+=", hackability=";
	if (is_hackable) status+="high";
	else status+="not so much";
	
	status+=".  Your name?\n";
	skt_sendN(s,&status[0],status.size());
	
	std::string name=skt_recv_line(s);
	if (is_hackable && intranet) {
		old_say_hello(&name[0]);
	} else {
		std::cout<<"Hello, "<<name<<"\n";
	}
	
	skt_close(s);
}


int main() {
	unsigned int port=1234;
	SERVER_SOCKET serv=skt_server(&port);
	while (1) { /* be a server */
		std::cout<<"Listening on port "<<port<<std::endl;
	
		skt_ip_t client_ip; unsigned int client_port=0;
		SOCKET s=skt_accept(serv,&client_ip,&client_port);
		std::cout<<"Client connected from "<<skt_print_ip(client_ip)<<":"<<client_port<<std::endl;
		
		bool intranet=false;
		if (client_ip.data[0]==127 || client_ip.data[0]==10) 
			intranet=true;

		try {
			handle_client(s,intranet);
		} 
		catch (std::runtime_error &e) {
			std::cout<<"Client error: "<<e.what()<<"\n";
		}
	}
}


