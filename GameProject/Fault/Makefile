SHELL =		/bin/sh
OS =		$(shell uname -s)

ifeq ($(OS),Darwin)
  # standard location for MacLab machines
  DOXYGEN =	/Applications/Doxygen.app/Contents/Resources/doxygen
else
  DOXYGEN =	/usr/bin/doxygen
endif

ifeq ($(OS),Darwin)
  CPPFLAGS = -I/usr/local/include -I/opt/local/include 
  LDFLAGS = -L/opt/local/lib -lm -L/usr/local/lib -lglfw -lpng -lglut -lGL -lGLU -framework OpenGL -framework Foundation
  CXX = clang++ -std=c++11
else
  CPPFLAGS = -I/usr/local/include
  LDFLAGS = —L/usr/local/lib -lpng -lglfw -lGL -lGLU -lm
  CXX = g++
endif

CXXFLAGS =	-g -Wall -pedantic

all: main

main: main.o
	$(CXX) $^ $(CXXFLAGS) -o main $(LDFLAGS) 

main.o: main.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) main.cxx 

.depend:
	-  $(CXX) $(CPPFLAGS) makedepend *.cxx

clean:
	rm -rf *.o main .depend
