#common makefile header

CC = gcc
CXX = g++

ARCH = $(shell uname -m)
#BIT32:=i686
#BIT64:=x86_64 

ifeq ($(ARCH),x86_64)
LIBSPATH=bin/x86_64
else
LIBSPATH=bin/i686
endif

PROG_NAME	= msc_test
SRCPATH=src
INCSPATH=include

SQLIBS = `pkg-config --libs sqlite3`

GLIBFLAGS = `pkg-config --cflags glib-2.0`
GLIBLIBS = `pkg-config --libs glib-2.0`

XCFLAGS  := `pkg-config --cflags x11`
XLIBS	:= `pkg-config --libs x11`

JSONFLAGS = `pkg-config --cflags jsoncpp`
JSONLIBS  = `pkg-config --libs jsoncpp`

WNCKFLAGS = `pkg-config --cflags libwnck-3.0`
WNCKLIBS  = `pkg-config --libs libwnck-3.0`

CFLAGS  = -Wall -g -L${LIBSPATH} -I${INCSPATH} ${JSONFLAGS} ${XCFLAGS} ${WNCKFLAGS}
LIBS	:= -L${LIBSPATH} -I${INCSPATH} ${XLIBS} ${JSONLIBS} ${WNCKLIBS} ${SQLIBS} \
-lmsc -ldl -lpthread -lasound  -lXtst

vpath %.cpp ${SRCPATH}

#OBJS := my_qtts.o read_wav.o
#OBJS := my_qisr.o my_alsa.o main_test.o parse_command.o send_keys.o
#OBJS := my_qisr.o my_alsa.o my_qtts.o parse_speech.o activate_win.o send_keys.o music_func.o exec_cmd.o parse_json.o read_wav.o my_init.o main.o
OBJS := my_qisr.o my_alsa.o my_qtts.o parse_speech.o activate_win.o send_keys.o\
 music_func.o exec_cmd.o search_database.o read_wav.o my_init.o list_window.o \
 main_thread.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CXX} -o $@ $^ ${LIBS}

%.o : %.cpp
	${CXX} ${CFLAGS} -c $<
	
%.o : %.c
	${CC} ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
