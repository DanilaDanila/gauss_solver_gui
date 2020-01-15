COMPILER=g++
STD=-std=c++17
I_DIR=
L_DIR=
OUTPUT=solver

all: main.o fract.o vector.hpp vector.cpp 
	$(COMPILER) *.o $(STD) -I$(I_DIR) -L$(L_DIR) -o $(OUTPUT)

main.o: main.cpp
	$(COMPILER) $(STD) -c main.cpp

fract.o: fract.hpp fract.cpp
	$(COMPILER) $(STD) -c fract.cpp -std=c++17

clean:
	rm *.o $(OUTPUT)
