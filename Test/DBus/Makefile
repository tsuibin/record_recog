#common makefile header

CC=gcc

PROG_NAME	= dbus

GLIB=`pkg-config --cflags --libs glib-2.0`
DBUS=`pkg-config --cflags --libs dbus-1 dbus-glib-1`
GIO=`pkg-config --cflags --libs gio-2.0`

CFLAGS  := -Wall -g ${DBUS} ${GLIB} ${GIO}
LIBS	:= ${DBUS} ${GLIB} ${GIO}  -lcrypt

#vpath %.cpp ${SRCPATH}

#OBJS := dbus1.o
#OBJS := method_call.o
#OBJS := giodbus1.o
OBJS := lockservice.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CC}   -o $@ $^ ${LIBS}

%.o : %.c
	${CC}   ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
