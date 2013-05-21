#common makefile header

CC=gcc

PROG_NAME = test
SRCPATH = ./

GTOPFLAGS = `pkg-config --cflags libgtop-2.0`
GTOPLIBS  = `pkg-config --libs libgtop-2.0`

CFLAGS  := -Wall -g ${GTOPFLAGS}
LIBS	:= ${GTOPLIBS}

vpath %.c ${SRCPATH}

OBJS := gtop_test.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CC}  -o $@ $^ ${LIBS}
	
%.o : %.c
	${CC}  ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
