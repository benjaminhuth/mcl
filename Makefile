ifndef CXX
CXX=g++
endif

ifndef DESTDIR
DESTDIR=/usr/local
endif

test.out: test.cpp
	git submodule update --init
	$(CXX) -std=c++17 test.cpp -g -o test.out
	
test: test.out
	./test.out --help --vectori 1 2 3 --singled 3.14
	cat three_vec.txt

install: 
	mkdir -p $(DESTDIR)/include/mcl && cp -rf mcl_basic.hpp mcl_arithmetic.hpp mcl_program_options.hpp mcl_tabular.hpp deps/ $(DESTDIR)/include/mcl

clean: 
	rm three_vec.txt test.out

.phony: install, test, clean
