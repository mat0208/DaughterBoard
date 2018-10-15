CXX=g++
CXXFLAGS=-W -Wall -ansi -pedantic -std=c++11
LDFLAGS=
EXEC=main_Pyranometer
SRC= main_Pyranometer.c
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main_Pyranometer: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main_Pyranometer.h

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			
