all:
	g++ -lsfml-window -lsfml-graphics -lsfml-system main.cc -o main

clean:
	rm -f main
