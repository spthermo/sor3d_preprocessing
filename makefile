OBJS = mapper.o
CC = g++
DEBUG = -ggdb
CFLAGS = -Wall $(DEBUG)

mapper : $(OBJS)
	
mapper.o : mapper.cpp Kinect.h
	$(CC) `pkg-config --cflags opencv` -o `basename mapper.cpp .cpp` mapper.cpp `pkg-config --libs opencv`
	
clean:
	\rm *.o mapper
