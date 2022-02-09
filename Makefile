all: main test

main: main.cpp ski.hpp

test: test.cpp ski.hpp

clean:
	rm -f test main

PHONY: clean