# Airport Management System Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# SFML libraries
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files (without main)
SOURCES = person.cpp crew.cpp flight.cpp airport.cpp 

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Targets
all: gui

# GUI version (Screens.cpp) - requires SFML
gui: $(SOURCES) Screens.cpp
	$(CXX) $(CXXFLAGS) -o airport_gui $(SOURCES) Screens.cpp $(SFML_LIBS)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f *.o airport_gui airport_gui.exe

# Run GUI version
run: gui
	./airport_gui

.PHONY: all clean gui run
