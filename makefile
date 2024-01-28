default: main.o
	g++ main.o -o "Cells evolution" -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm *.o "Cells evolution"