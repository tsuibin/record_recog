#common makefile header

CC=gcc
CCX=g++

PROG_NAME	= dbus_test

CFLAGS  := `pkg-config --cflags dbus-1`
LIBS	:= `pkg-config --libs dbus-1`

#vpath %.cpp ${SRCPATH}

#OBJS := dbus1.o
OBJS := method_call.o
#OBJS := my_qisr.o test_qisr.o
#OBJS := my_qisr.o my_alsa.o test_record.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CCX}   -o $@ $^ ${LIBS}

%.o : %.cpp
	${CCX} -m32 -c ${CFLAGS} $<
	
%.o : %.c
	${CC}   ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
