all: main.cc
	g++ -lsfml-graphics -lsfml-window -lsfml-system -lm -o main main.cc

clean:
	rm -f main
