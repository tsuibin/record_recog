#common makefile header

CC=gcc

PROG_NAME = Test
SRCPATH = ./

SQLIBS = `pkg-config --libs sqlite3`

CFLAGS  := -Wall -g
LIBS	:= ${SQLIBS}

vpath %.c ${SRCPATH}

OBJS := test1.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CC}  -o $@ $^ ${LIBS}
	
%.o : %.c
	${CC}  ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
