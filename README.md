# PN-SFML Hangman
A simple, configurable C++ SFML implementation of the popular word-based game. The project was developed as an assignment for a Low-level Programming class at the Poznan University of Technology.

## Installation
Run the following commands to install the game:

`git clone https://gitlab.com/put-cs/pn-sfml-hangman.git`

Run it with:

`cd pn-sfml-hangman`

`./hangman&`

The game is ready to play, but you might want to take advantage of the various configuration options included.

## Configuration
Modify `config.hangman`.

The syntax is self-explanatory.
The changes take effect without a recompilation, but if something goes wrong, you'll have to use the `make` command included in the `base-devel` package group in your distribution's repo.

You'll also need the SFML 2.5.1 library installed on your system. This is the only version that the game was tested for.

## Compatibility
GNU/Linux only.

## Authors
Michał Miłek, Sebastian Nowak

## Licensing
The game is licensed under the GNU GPLv3.
