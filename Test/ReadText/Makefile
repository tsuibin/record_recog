#common makefile header

CC=gcc
CCX=g++

PROG_NAME	= read-text
SRCPATH=./
INCSPATH=../../include
LIBSPATH=../../bin

CFLAGS  = -Wall -g -L${LIBSPATH} -I${INCSPATH}
LIBS	:= -L${LIBSPATH} -I${INCSPATH} -lmsc -ldl -lpthread -lasound

vpath %.cpp ${SRCPATH}

OBJS := my_qtts.o read_wav.o my_init.o main.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CCX} -m32 -o $@ $^ ${LIBS}

%.o : %.cpp
	${CCX} -m32 ${CFLAGS} -c $<
	
%.o : %.c
	${CC} -m32 ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
