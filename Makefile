CFLAGS = -g
OBJS = demo.o coordinates.o ephemeris_line.o
LIBS = -lboost_thread

all: $(OBJS)
	g++ ${OBJS} ${LIBS} -o demo


-include $(OBJS:.o=.d)

.cpp.o:
	g++ ${CFLAGS} -c $< -o $@
	g++ ${CFLAGS} -MM $< > $*.d


clean:
	rm -f *.o *~ *.d demo