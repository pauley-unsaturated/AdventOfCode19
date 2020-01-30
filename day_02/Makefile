
CC=clang
CXX=clang++

LD=clang++

CFLAGS= -g -O0
CXXFLAGS= $(CFLAGS) -std=c++17
LDFLAGS=

all: day_02

day_02: main.o
	$(LD) $(LDFLAGS) $< -o $@

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o *~ day_02
