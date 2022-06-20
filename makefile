SRC   = $(wildcard src/*.cc)
DEPS  = $(wildcard headers/*.hh)
DEPS += $(wildcard src/*.hh)
OBJ   = $(addsuffix .o,$(subst src/,bin/,$(basename $(SRC))))

LIB    = ./bin/libamaretto
STATIC = .a
SHARED = .so
ifeq ($(OS),Windows_NT)
	SHARED = .dll
endif

AR = ar

CXX = g++
CXX_VER = c++14
CXX_FLAGS = -O3 -std=$(CXX_VER) -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations

ifeq ($(OS),Windows_NT)
	CLEAN = del .\bin.\*

	INSTALL = echo install is linux only
else
	CLEAN = rm -r -f bin/*

	INSTALL = \
		cp ./bin/libamaretto.* /usr/lib/ && \
		mkdir -p /usr/include/amaretto && cp ./headers/* /usr/include/amaretto/

	WIDECHAR = true

	CXX_LIBS = -lncursesw
endif

shared: ./bin $(OBJ) $(SRC)
	$(CXX) -shared -fPIC $(CXX_FLAGS) -o $(LIB)$(SHARED) $(OBJ) $(CXX_LIBS)

static: ./bin $(OBJ) $(SRC)
	$(AR) rcs $(LIB)$(STATIC) $(OBJ)

bin/%.o: src/%.cc $(DEPS)
	$(CXX) -c -fPIC $< $(CXX_FLAGS) -o $@ $(CXX_LIBS)

tests: ./bin $(OBJ) $(LIB)*
	$(CXX) ./tests/graphics.cc -o ./bin/graphics -L./bin/ -lamaretto $(CXX_LIBS)
	$(CXX) ./tests/mouse.cc -o ./bin/mouse -L./bin/ -lamaretto $(CXX_LIBS)

examples: ./bin $(OBJ) $(LIB)*
	$(CXX) ./examples/painting.cc -o ./bin/painting -L./bin/ -lamaretto $(CXX_LIBS)

./bin:
	mkdir -p bin

clean:
	$(CLEAN)

install:
	$(INSTALL)

all:
	@echo shared, static, tests, examples, clean, install
