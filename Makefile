ifndef CXX
CXX=g++
endif

ifndef DESTDIR
DESTDIR=/usr/local
endif

test.out:
	(cd dep/optional && git submodule update --init)
	$(CXX) -std=c++14 test.cpp -o test.out
	
test: test.out
	./test.out
	cat three_vec.txt

install: 
	mkdir -p $(DESTDIR)/include/mcl && cp -rf mcl_basic.hpp mcl_program_options.hpp mcl_tabular.hpp dep/ $(DESTDIR)/include/mcl

clean: 
	rm three_vec.txt test.out

.phony: install, test, clean
