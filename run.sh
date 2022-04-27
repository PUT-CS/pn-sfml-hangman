#!/bin/bash
g++ -c main.cpp
g++ main.o -o hangman-app -lsfml-graphics -lsfml-window -lsfml-system
rm -f main.o
./hangman-app&
