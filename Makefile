CC=g++
LIBSFML=-lsfml-graphics -lsfml-window -lsfml-system

.PHONY: test clean

test: hangman-app
	./hangman-app&

hangman-app: main.o
	$(CC) main.o -o hangman-app $(LIBSFML)

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm -f main.o