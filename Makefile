CC=g++ 
CFLAGS=-ansi -pedantic -Wno-deprecated -std=c++0x -Wall -pedantic -O3
INC=-I$(EIGEN3_INCLUDE_DIR) -I./ 
LIB=-DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU
name=

OBJECT = Box.o BRDF.o Image.o main.o Parse.o Plane.o Ray.o Scene.o Sphere.o Shape.o Triangle.o unit_tests.o VectorMath.o Finish.o Tokens.o Light.o Camera.o

ifdef DEBUG
	CFLAGS += -D DEBUG
endif

ifdef UNIT
	CFLAGS += -D UNIT_TEST
	name +=raytrace_test
else
	name += raytrace
endif

all: $(OBJECT)
	touch main.cpp
	$(CC) -g $(CFLAGS) $(INC) $(OBJECT) $(LIB) -o $(name)

%.o: %.cpp
	$(CC) -g -c $< $(CFLAGS) $(INC) $(LIB)

%.cpp: %.h
	touch $@
	
%.cpp: %.hpp
	touch $@

simple:
	./raytrace 640 480 resources/simple.pov
ball:
	./raytrace resources/bunny_small.pov
tri:
	./raytrace resources/bunny_small_tris.pov
good:
	./raytrace 640 480 resources/simp_cam.pov
good2:
	./raytrace 640 480 resources/simp_cam2.pov
clean:
	rm -f *~ *.o a.out raytrace raytrace_test
clear: $(OBJECT)
	clear
	rm -f *~ *.o a.out raytrace raytrace_test
	$(CC) $(CFLAGS) $(INC) *.cpp $(LIB) -o raytrace
fast: $(OBJECT)
	rm -f *~ *.o a.out raytrace raytrace_test
	clear
	$(CC) $(CFLAGS) $(INC) *.cpp $(LIB) -o raytrace
	./raytrace resources/bunny_small.pov
