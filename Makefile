#common makefile header

CC=gcc
CCX=g++

PROG_NAME	= msc_test
SRCPATH=src
INCSPATH=include
LIBSPATH=bin

XCFLAGS  := `pkg-config --cflags x11`
XLIBS	:= `pkg-config --libs x11`

JSONFLAGS = `pkg-config --cflags jsoncpp`
JSONLIBS  = `pkg-config --libs jsoncpp`

CFLAGS  = -Wall -g -L${LIBSPATH} -I${INCSPATH} ${JSONFLAGS} ${XCFLAGS}
LIBS	:= -L${LIBSPATH} -I${INCSPATH} ${XLIBS} ${JSONLIBS} -lmsc -ldl -lpthread -lasound  -lXtst

vpath %.cpp ${SRCPATH}

#OBJS := my_qtts.o read_wav.o
#OBJS := my_qisr.o my_alsa.o main_test.o parse_command.o send_keys.o
OBJS := my_qisr.o my_alsa.o parse_speech.o activate_win.o send_keys.o music_func.o exec_cmd.o parse_json.o main.o

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
