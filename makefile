default: main.o
	g++ main.o -o "Prey-Predator" -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm *.o "Prey-Predator"