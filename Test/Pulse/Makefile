#common makefile header

CC=gcc

PROG_NAME	= recd

GLIB=`pkg-config --cflags --libs glib-2.0`
PULSE_SIMPLE=`pkg-config --cflags --libs libpulse-simple`
PULSE_MAINLOOP=`pkg-config --cflags --libs libpulse-mainloop-glib`

CFLAGS  = -Wall -g
LIBS	:= ${GLIB} ${PULSE_SIMPLE} ${PULSE_MAINLOOP} -lpthread

vpath %.c ${SRCPATH}

#OBJS := test1.o
OBJS := record.o get_db.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CC}  -o $@ $^ ${LIBS}
	
%.o : %.c
	${CC}  ${CFLAGS} -c $< ${LIBS}

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
