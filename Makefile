CC=g++
CFLAGS=-ansi -pedantic -Wno-deprecated -std=c++0x -Wall -pedantic -O3
INC=-I$(EIGEN3_INCLUDE_DIR) -I./ 
LIB=-DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU
name=

OBJECT = BoundingBox.o Box.o BRDF.o BVH.o Camera.o Finish.o Image.o Light.o LoadingBar.o main.o Parse.o Plane.o Ray.o Scene.o Sphere.o Shape.o Tokens.o Triangle.o

ifdef DEBUG
	CFLAGS += -D DEBUG
endif

ifdef UNIT
	CFLAGS += -D UNIT_TEST
	name +=raytrace_test
	OBJECT += unit_tests.o
else
	name += raytrace
endif

ifdef PROFILE
	CFLAGS += -pg
endif

ifdef ICPC
	CC=ICPC
else
	CC=g++
endif

all: $(OBJECT)
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
simpleGI:
	./raytrace 320 240 resources/simple-gi.pov 0 2
cornell:
	./raytrace 640 480 resources/RScornell.pov 0 2
cornellAA:
	./raytrace 640 480 resources/RScornell.pov 0 2
balls2:
	./raytrace 640 480 resources/balls2.pov 0 0 0
recurses:
	./raytrace 640 480 resources/recurses.pov 0 0 0
partfive:
	./raytrace 640 480 resources/simple-gi.pov 0 2
	./raytrace 640 480 resources/pretty.pov 0 2
	./raytrace 640 480 resources/RScornell.pov 0 2
	./raytrace 640 480 resources/abduction.pov 0
clean:
	rm -f *~ *.o a.out raytrace raytrace_test gmon.out resources/gprof*
