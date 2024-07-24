all: program1 program2

program1: program1.cpp
	g++ program1.cpp -o program1 -lpthread


program2: program2.cpp
	g++ program2.cpp -o program2

run1: program1
	./program1 

run2: program2
	./program2

rm:
	rm -f program1 program2
