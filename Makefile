CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

MAIN_SRC = T2SEM2.cpp
MAIN_EXE = balatro

all: $(MAIN_EXE)

$(MAIN_EXE): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) $(MAIN_SRC) -o $(MAIN_EXE)

run: $(MAIN_EXE)
	./$(MAIN_EXE)

clean:
	rm -f $(MAIN_EXE)

.PHONY: all run clean