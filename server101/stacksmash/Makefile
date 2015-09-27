OPTS=-Os
EASYMODE= -fno-stack-protector  -z execstack 
CFLAGS=$(OPTS) $(EASYMODE) -m32

all: server

server: server.cpp
	g++ $(CFLAGS) $< -o $@

sploit: sploit.cpp
	g++ $(CFLAGS) $< -o $@

clean:
	- rm server
