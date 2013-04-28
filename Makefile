#common makefile header

CC=gcc
CCX=g++

PROG_NAME	= msc_test
SRCPATH=src
INCSPATH=include
LIBSPATH=bin

XCFLAGS  := `pkg-config --cflags x11`
XLIBS	:= `pkg-config --libs x11`

LIBS	:= -L${LIBSPATH} -I${INCSPATH} -lmsc -ldl -lpthread -lasound

vpath %.cpp ${SRCPATH}

#OBJS := test_alsa.o my_alsa.o
#OBJS := my_qisr.o test_qisr.o
#OBJS := my_qisr.o my_alsa.o record_test.o
#OBJS := my_qisr.o my_alsa.o main_test.o parse_command.o send_keys.o
OBJS := my_qisr.o my_alsa.o record_recog.o parse_command.o send_keys.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CCX} -m32 -o $@ $^ ${LIBS} ${XLIBS} -lXtst

%.o : %.cpp
	${CCX} -m32 ${XCFLAGS} -c $< ${LIBS}
	
%.o : %.c
	${CC} -m32 ${XCFLAGS} -c $< ${LIBS}

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
