CXX=clang++
CFLAGS=-std=c++11 -Wall -c
OFLAGS=-o
CPP_MAIN=main.cpp


all: $(CPP_MAIN) $(CPP_SOURCES)
	$(CXX) $(OFLAGS) t.out $(CPP_MAIN)

test:
	$(CXX) $(FLAGS) $(name) -o t.out

