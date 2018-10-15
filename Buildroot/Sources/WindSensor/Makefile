CXX=g++
CXXFLAGS=-W -Wall -ansi -pedantic -std=c++11
LDFLAGS=
EXEC=main_WindSensor
SRC= main_WindSensor.c
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main_WindSensor: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main_WindSensor.h

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			
