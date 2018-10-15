CXX=g++
CXXFLAGS=-W -Wall -ansi -pedantic -std=c++11
LDFLAGS=
EXEC=main_Lidar
SRC= main_Lidar.c
OBJ= $(SRC:.c=.o)

all: $(EXEC)

main_Lidar: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o: main_Lidar.h

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
			
