#common makefile header

CC=gcc

PROG_NAME = wnck_test
SRCPATH = ./

WNCKFLAGS = `pkg-config --cflags libwnck-3.0`
WNCKLIBS  = `pkg-config --libs libwnck-3.0`

CFLAGS  := -Wall -g ${WNCKFLAGS}
LIBS	:= ${WNCKLIBS}

vpath %.c ${SRCPATH}

OBJS := listwin.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CC}  -o $@ $^ ${LIBS}
	
%.o : %.c
	${CC}  ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
