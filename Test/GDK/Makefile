#common makefile header

CC=gcc

PROG_NAME = wnck_test
SRCPATH = ./

XLIBCFLAGS = `pkg-config --cflags x11`
XLIBLIBS = `pkg-config --libs x11`

GTOPFLAGS = `pkg-config --cflags libgtop-2.0`
GTOPLIBS  = `pkg-config --libs libgtop-2.0`

WNCKFLAGS = `pkg-config --cflags libwnck-3.0`
WNCKLIBS  = `pkg-config --libs libwnck-3.0`

CFLAGS  := -Wall -g ${WNCKFLAGS} ${XLIBCFLAGS} ${GTOPFLAGS}
LIBS	:= ${WNCKLIBS} ${XLIBLIBS} ${GTOPLIBS}

vpath %.c ${SRCPATH}

OBJS := filter.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CC}  -o $@ $^ ${LIBS}
	
%.o : %.c
	${CC}  ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
