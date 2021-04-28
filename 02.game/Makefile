SOURCES=\
 main.cpp
#  view.cpp\
#  cli_view.cpp\
#  game.cpp\
#  snake_game.cpp\
#  console.cpp

EXTRA_FLAGS=-std=c++17

.PHONY: all snake run depend clean

all:
	make depend && make snake

snake: $(SOURCES:.cpp=.o)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

run: snake
	./snake

%.o: %.cpp
	$(CXX) $(EXTRA_FLAGS) $(CXXFLAGS) -c $< -o $@

depend:
	$(CXX) $(EXTRA_FLAGS) $(SOURCES) -MM > .depend

clean:
	rm -f snake .depend

include .depend
