OPTS=-Os
CFLAGS=$(OPTS)

all: demo

demo: demo.cpp
	g++ $(CFLAGS) $< -o $@

bank: bank.cpp
	g++ $(CFLAGS) $< -o $@

clean:
	- rm demo
