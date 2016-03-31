CC=g++ 
CFLAGS=-ansi -pedantic -Wno-deprecated -std=c++0x -Wall -pedantic -O3
INC=-I$(EIGEN3_INCLUDE_DIR) -I./ 
LIB=-DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU

OBJECT = Box.o Image.o main.o Parse.o Plane.o Ray.o Scene.o Sphere.o Shape.o Triangle.o Tokens.o VectorMath.o

ifdef DEBUG
	CFLAGS += -D DEBUG
endif

ifdef UNIT
	CFLAGS += -D UNIT_TEST
endif

all: $(OBJECT)
	$(CC) -g $(CFLAGS) $(INC) $(OBJECT) $(LIB) -o rt

%.o: %.cpp
	$(CC) -g -c $< $(CFLAGS) $(INC) $(LIB)

%.cpp: %.h
	touch $@
	
%.cpp: %.hpp
	touch $@

simple:
	./rt 640 480 resources/simple.pov
ball:
	./rt resources/bunny_small.pov
tri:
	./rt resources/bunny_small_tris.pov
good:
	./rt 640 480 resources/simp_cam.pov
good2:
	./rt 640 480 resources/simp_cam2.pov
clean:
	rm -f *~ *.o a.out rt
clear: $(OBJECT)
	clear
	rm -f *~ *.o a.out rt
	$(CC) $(CFLAGS) $(INC) *.cpp $(LIB) -o rt
fast: $(OBJECT)
	rm -f *~ *.o a.out rt
	clear
	$(CC) $(CFLAGS) $(INC) *.cpp $(LIB) -o rt
	./rt resources/bunny_small.pov
