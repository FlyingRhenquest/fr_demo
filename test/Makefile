CFLAGS = -I.. -g
OBJS = btree_test.o timetree_test.o coordinates_test.o jd_test.o gmst_test.o ephemeris_line_test.o ephemeris_cache.o sp3_reader_test.o socket_server_test.o run_tests.o
LIBS = -lcppunit -lboost_thread
EXT_OBJS = ../coordinates.o ../ephemeris_line.o

run_tests: ${OBJS}
	g++ ${CFLAGS} ${OBJS} ${LIBS} ${EXT_OBJS} -o run_tests

-include $(OBJS:.o=.d)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@
	g++ $(CFLAGS) -MM $< > $*.d

clean: 
	rm -f run_tests *.o *.d *~