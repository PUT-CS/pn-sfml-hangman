CC=g++
LIBSFML=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
HEADERS=src/logic.cpp src/sfmlops.cpp src/stringops.cpp src/config.cpp

.PHONY: test clean

test: hangman-app
	./hangman-app&

hangman-app: main.o
	$(CC) main.o -o hangman-app $(HEADERS) $(LIBSFML)

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm -f main.o
