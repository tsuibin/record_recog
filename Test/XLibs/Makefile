#common makefile header

CC=gcc
CCX=g++

PROG_NAME	= xlib_test

GLIBFLAGS = `pkg-config --cflags glib-2.0`
GLIBLIBS = `pkg-config --libs glib-2.0`

CFLAGS  := -Wall -g `pkg-config --cflags x11` ${GLIBFLAGS}
LIBS	:= `pkg-config --libs x11` ${GLIBLIBS}

#vpath %.cpp ${SRCPATH}

OBJS := xlib2.o
#OBJS := get_pro.o
#OBJS := my_qisr.o test_qisr.o
#OBJS := my_qisr.o my_alsa.o test_record.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CCX} -o $@ $^ ${LIBS} -lXtst

%.o : %.cpp
	${CCX} -c ${CFLAGS} $<
	
%.o : %.c
	${CC} ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
