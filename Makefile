all: main test

main: main.cpp ski.hpp

test: test.cpp ski.hpp skilib.hpp

clean:
	rm -f test main

PHONY: clean