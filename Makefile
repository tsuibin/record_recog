#common makefile header

CC=gcc
CCX=g++

BASEDIR = .
#include configure.mk

PROG_NAME	= msc_test

LIBS	:= -Lbin -Iinclude -lmsc -ldl -lpthread -lasound

#OBJS := test_alsa.o my_alsa.o
#OBJS := my_qisr.o test_qisr.o
#OBJS := my_qisr.o my_alsa.o test_record.o
OBJS := my_qisr.o my_alsa.o record_recog.o parse_command.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CCX} -m32 -o $@ $^ ${LIBS}

%.o : %.cpp
	${CCX} -m32 -c $< ${LIBS}
	
%.o : %.c
	${CC} -m32 -c $< ${LIBS}

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
