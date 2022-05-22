# PN-SFML Hangman
A simple, configurable C++ SFML implementation of the popular word-based game. The project was developed as an assignment for a Low-level Programming class at the Poznan University of Technology.

## Installation

1. Install the SFML 2.5.1 library. This is the version that the game was developed on. Others might work too.

2. Run the following commands to install the game:

	`git clone https://gitlab.com/put-cs/pn-sfml-hangman.git`

3. Run it with:

	`cd pn-sfml-hangman`

	`./hangman&`

The game is ready to play, but you might want to take advantage of the various configuration options included.

## Configuration
Modify `config.hangman`.

The syntax is self-explanatory.
The changes take effect without a recompilation, but if something goes wrong, you'll have to use the `make` command included in the `base-devel` package group in your distribution's repo.

## Wordlists

The game comes packaged with two huge wordlist - in Polish and English.

To use your own wordlist, put them in `assets/wordlists` and run `unix2dos` on the file just in case it originated on a Windows machine. `unix2dos` is found under `util/` as a binary.

## Compatibility
GNU/Linux only.

## Authors
Michał Miłek, Sebastian Nowak

## Licensing
The game is licensed under the GNU GPLv3.
