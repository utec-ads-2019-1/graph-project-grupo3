OBJS = sfml.o
CXX = g++
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

graphs:
	$(CXX) $(OBJS) $(LDLIBS)

sfml.o: sfml.cpp $(LDLIBS)