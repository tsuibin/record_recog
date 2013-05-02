#common makefile header

CC=gcc
CCX=g++

PROG_NAME	= xlib_test

CFLAGS  := `pkg-config --cflags x11`
LIBS	:= `pkg-config --libs x11`

#vpath %.cpp ${SRCPATH}

OBJS := xlib1.o
#OBJS := get_pro.o
#OBJS := my_qisr.o test_qisr.o
#OBJS := my_qisr.o my_alsa.o test_record.o

all : ${PROG_NAME}

${PROG_NAME} : ${OBJS}
	${CCX} -m32  -o $@ $^ ${LIBS} -lXtst

%.o : %.cpp
	${CCX} -m32 -c ${CFLAGS} $<
	
%.o : %.c
	${CC} -m32  ${CFLAGS} -c $<

clean :
	rm -rf ${PROG_NAME} *.o

rebuild : clean all
