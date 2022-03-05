INCDIR = include/
LIBDIR = lib/
LDFLAGS = -lfmt

all:
	g++ -I ${INCDIR} -L ${LIBDIR} src/util/logger.cpp src/util/bit_op.cpp src/util/register.cpp src/memory.cpp src/cpu.cpp src/main.cpp -o bin/gb.app ${LDFLAGS}

clean:
	rm bin/gb.app

run:
	./bin/gb.app