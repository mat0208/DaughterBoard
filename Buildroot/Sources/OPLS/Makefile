CXX=g++
CXXFLAGS=-W -Wall -ansi -pedantic -std=c++11
LDFLAGS=-lpthread
EXEC=main_OPLS
SRC= main_OPLS.c
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main_OPLS: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main_OPLS.h

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			
