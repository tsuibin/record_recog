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

#OBJS := my_qisr.o my_alsa.o main_test.o parse_command.o send_keys.o
OBJS := my_qisr.o my_alsa.o parse_speech.o activate_win.o send_keys.o music_func.o exec_cmd.o main.o

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
