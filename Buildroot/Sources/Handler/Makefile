CXX=g++
CXXFLAGS=-W -Wall -ansi -pedantic -std=c++11
LDFLAGS=-lpthread
EXEC=main_Handler
SRC= main_Handler.c
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main_Handler: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main_Handler.h

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			

