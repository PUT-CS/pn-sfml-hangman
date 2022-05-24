CC=g++
LIBSFML=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
HEADERS=src/logic.cpp src/sfmlops.cpp src/stringops.cpp src/config.cpp src/fileops.cpp src/websearch.cpp

.PHONY: test clean

test: hangman
	./hangman

hangman: src/main.o
	$(CC) src/main.o -o hangman $(HEADERS) $(LIBSFML)
	rm -f src/main.o

main.o: src/main.cpp
	$(CC) -c src/main.cpp

clean:
	rm -f src/main.o
