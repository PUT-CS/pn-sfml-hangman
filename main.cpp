#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <string.h>
#include <fstream>
#include <array>
#include "src/logic.hpp"
#include "src/stringops.hpp"
#include "src/sfmlops.hpp"
#include "src/config.hpp"
#include "src/fileops.hpp"
#include <memory>
#include <stdexcept>

#define MAX_LETTERS 16
#define MIN_LETTERS 5
#define SCREEN_X 1000
#define SCREEN_Y 1000

int mode = 0;
Config CONFIG;
// 0 - menu
// 1 - gra
// 2 - win screen
// 3 - human input
// 4 - load save
// 5 - saving
// 6 - lose screen
// 7 - about

int main(void){
    srand(time(NULL));

    CONFIG = getConfig();

    char* dirty_word;
    std::wstring word;
    std::wstring word_hidden;
    std::wstring used = L"";
    std::wstring letter;
    std::wstring humanword=L"";
    std::string savefile_name="";
    std::string newsave_name="";
    float fails = 0;
    float correct_guesses = 0;
    int prev_mode;

    sf::RenderWindow window(sf::VideoMode(SCREEN_X,SCREEN_Y), CONFIG.WINDOW_NAME);
    sf::Font font;
    std::string fontnamestr(CONFIG.FONT_FILE.begin(), CONFIG.FONT_FILE.end());

    if(!font.loadFromFile("assets/fonts/" + fontnamestr)){
        puts("COULDN'T LOAD FONT");
        exit(1);
    }

    sf::Music music;
    std::string musicnamestr(CONFIG.MUSIC_NAME.begin(), CONFIG.MUSIC_NAME.end());
    std::string musicpath = "assets/music/" + musicnamestr;
    music.setVolume(100);

    if (!music.openFromFile(musicpath)) {
        puts("COULDN'T LOAD MUSIC");
        exit(1);
    }
    music.setLoop(1);
    if (CONFIG.MUSIC_ON)
        music.play();

    sf::SoundBuffer correctbuffer;
    if (!correctbuffer.loadFromFile("assets/sounds/correct.wav")) {
        puts("COULDN'T LOAD CORRECT.WAV");
        exit(1);
    }
    sf::Sound correct;
    correct.setBuffer(correctbuffer);
    correct.setVolume(100);

    sf::SoundBuffer incorrectbuffer;
    if (!incorrectbuffer.loadFromFile("assets/sounds/incorrect.wav")) {
        puts("COULDN'T LOAD INCORRECT.WAV");
        exit(1);
    }
    sf::Sound incorrect;
    incorrect.setBuffer(incorrectbuffer);
    incorrect.setVolume(100);

    sf::Event event;
    sf::Color BackgroundColor(CONFIG.BG_R, CONFIG.BG_G, CONFIG.BG_B);
    sf::Color FontColor(CONFIG.FNT_R, CONFIG.FNT_G, CONFIG.FNT_B);

    sf::Text SFtitle;
    SFtitle = applyStyle(SFtitle, font, 80, FontColor, CONFIG.FNT_MULTIPLIER);
    SFtitle.setString(L"");
    SFtitle.setString(L"Hangman");
    SFtitle = center(SFtitle, 2.0f, 7.0f);

    sf::Text SFmain_menu;
    SFmain_menu = applyStyle(SFmain_menu, font, 56, FontColor, CONFIG.FNT_MULTIPLIER);
    SFmain_menu.setString(L"Main Menu");
    SFmain_menu = center(SFmain_menu, 2.0f, 2.8f);

    sf::Text SFused;
    SFused = applyStyle(SFused, font, 25, FontColor, CONFIG.FNT_MULTIPLIER);
    SFused.setLetterSpacing(1.7f);
    SFused = center(SFused, 2.0f, 1.3f);

    sf::Text SFauthors;
    SFauthors = applyStyle(SFauthors, font, 35, FontColor, CONFIG.FNT_MULTIPLIER);
    SFauthors.setString(L"Michał Miłek & Sebastian Nowak");
    SFauthors = center(SFauthors, 2.0f, 2.0f);

    sf::Text SFlicense;
    SFlicense = applyStyle(SFlicense, font, 35, FontColor, CONFIG.FNT_MULTIPLIER);
    SFlicense.setString(L"Licensed under GNU GPLv3");
    SFlicense = center(SFlicense, 2.0f, 1.3f);

    sf::Text SFinfo;
    SFinfo = applyStyle(SFinfo, font, 30, FontColor, CONFIG.FNT_MULTIPLIER);
    SFinfo.setString(L"A simple, configurable C++ SFML implementation");
    SFinfo = center(SFinfo, 2.0f, 4.0f);

    sf::Text SFyear;
    SFyear = applyStyle(SFyear, font, 35, FontColor, CONFIG.FNT_MULTIPLIER);
    SFyear.setString(L"2022");
    SFyear = center(SFyear, 2.0f, 1.8f);

    sf::Text SFfails;
    SFfails = applyStyle(SFfails, font, 30, FontColor, CONFIG.FNT_MULTIPLIER);
    SFfails.setString("2022");
    SFfails = center(SFfails, 2.0f, 1.4f);

    sf::Text new_game;
    new_game = applyStyle(new_game, font, 30, FontColor, CONFIG.FNT_MULTIPLIER);
    new_game.setString("New Game");
    new_game = center(new_game, 2.0f, 2.03f);

    sf::Text load_save;
    load_save = applyStyle(load_save, font, 30, FontColor, CONFIG.FNT_MULTIPLIER);
    load_save.setString("Load Save");
    load_save = center(load_save, 2.0f, 1.52f);

    sf::Text SFword_hidden;
    SFword_hidden = applyStyle(SFword_hidden, font, 50, FontColor, CONFIG.FNT_MULTIPLIER);
    SFword_hidden.setString(L"");
    SFword_hidden.setLetterSpacing(3.0f);
    SFword_hidden = center(SFword_hidden, 2.0f, 2.0f);

    sf::Text used_letters;
    used_letters = applyStyle(used_letters, font, 50, FontColor, CONFIG.FNT_MULTIPLIER);
    used_letters.setString(used);
    used_letters.setLetterSpacing(3.0f);
    used_letters = center(used_letters, 2.0f, 1.10f);

    sf::Text SFendfails;
    SFendfails = applyStyle(SFendfails, font, 40, FontColor, CONFIG.FNT_MULTIPLIER);
    SFendfails.setString(std::to_string(fails));
    SFendfails = center(SFendfails, 2.0f, 1.3f);

    sf::Text SFcorrect_incorrect_ratio;
    SFcorrect_incorrect_ratio = applyStyle(SFcorrect_incorrect_ratio, font, 40, FontColor, CONFIG.FNT_MULTIPLIER);
    SFcorrect_incorrect_ratio = center(SFcorrect_incorrect_ratio, 2.0f, 1.1f);

    sf::Text SFendcorrect_guesses;
    SFendcorrect_guesses = applyStyle(SFendcorrect_guesses, font, 40, FontColor, CONFIG.FNT_MULTIPLIER);
    SFendcorrect_guesses.setString(std::to_string(correct_guesses));
    SFendcorrect_guesses = center(SFendcorrect_guesses, 2.0f, 1.2f);

    sf::Text SFword_reveal;
    SFword_reveal = applyStyle(SFword_reveal, font, 40, FontColor, CONFIG.FNT_MULTIPLIER);
    SFword_reveal = center(SFword_reveal, 2.0f, 1.7f);

    sf::Text SFinput_prompt;
    SFinput_prompt = applyStyle(SFinput_prompt, font, 30, FontColor, CONFIG.FNT_MULTIPLIER);
    SFinput_prompt.setString("Input the word for the other player to guess");
    SFinput_prompt = center(SFinput_prompt, 2.0f, 2.0f);

    sf::Text SFhuman_input;
    SFhuman_input = applyStyle(SFhuman_input, font, 25, FontColor, CONFIG.FNT_MULTIPLIER);
    SFhuman_input.setString(L"");
    SFhuman_input = center(SFhuman_input, 2.0f, 1.8f);

    sf::Text SFgoback_prompt;
    SFgoback_prompt = applyStyle(SFgoback_prompt, font, 25, FontColor, CONFIG.FNT_MULTIPLIER);
    SFgoback_prompt.setString("Press ENTER to go back");
    SFgoback_prompt = center(SFgoback_prompt, 2.0f, 1.1f);

    sf::Text SFkeybinds;
    SFkeybinds = applyStyle(SFkeybinds, font, 16, FontColor, CONFIG.FNT_MULTIPLIER);
    SFkeybinds.setString(
    "New game (CPU): CTRL+N\n"
    "New game (HUM): CTRL+H\n"
    "Load Game:      CTRL+L\n"
    "Mute Sounds:    CTRL+Subtract\n"
    "Unmute Sounds:  CTRL+Plus\n"
    "About:          CTRL+A\n"
    "Quit:           CTRL+Q\n"
    );
    SFkeybinds = center(SFkeybinds, 5.0f, 1.1f);

    while (window.isOpen()) {
        while (window.waitEvent(event)){

            if (event.type == sf::Event::Closed){
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                if (mode !=7) {
                    prev_mode = mode;
                    mode = 7;
                }
            }

            if (mode == 7 && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                mode = prev_mode;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen)) {
                music.setVolume(0);
                correct.setVolume(0);
                incorrect.setVolume(0);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
                music.setVolume(100);
                correct.setVolume(100);
                incorrect.setVolume(100);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                humanword.clear();
                SFhuman_input.setString(L"");
                SFtitle.setString("New game");
                SFinput_prompt.setString("Input a word for the other player to guess");
                mode = 3; // get human input for the word
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::N)) { // restart
                dirty_word = getRandomWord2(CONFIG.WORDLIST);
                // dirty_word = getRandomWord();
                std::string wordstr(dirty_word, dirty_word + strlen(dirty_word)-1);
                word = stringToWstring(wordstr);
                word_hidden = hideWord(word);
                SFword_hidden.setString(word_hidden);
                SFword_hidden = center(SFword_hidden, 2.0f, 2.0f);
                fails = 0;
                correct_guesses = 0;
                used.clear();
                mode = 1; // start the game
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                SFhuman_input.setString("");
                savefile_name.clear();
                mode = 4; // load save
            }

            if (word.size()!=0 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                newsave_name.clear();
                SFhuman_input.setString("");
                mode = 5;
            }

            if(event.type == sf::Event::TextEntered){

                if (mode == 3){ //user word input
                    if (event.text.unicode != '\r') {

                        if (event.text.unicode == '\b' && humanword.size() > 0) { //backspace
                            humanword.erase(humanword.size()-1, 1); // delete the last character
                            SFhuman_input.setString(humanword);
                        }
                        else if (humanword.size() <= MAX_LETTERS  && event.text.unicode != ' ' && event.text.unicode != '\b' && isLetterAllowed(event.text.unicode)) { //normal letters
                            humanword.push_back(static_cast<wchar_t>(event.text.unicode));
                            SFhuman_input.setString(humanword);
                        }
                        SFhuman_input = center(SFhuman_input, 2.0f, 1.8f);
                    }
                    else if (humanword.size() >= MIN_LETTERS){
                        word = humanword;
                        SFtitle.setString(word);
                        word_hidden = hideWord(word);
                        SFword_hidden.setString(word_hidden);
                        SFword_hidden = center(SFword_hidden, 2.0f, 2.0f);
                        mode = 1;
                    }
                }

                if (mode == 5) { // saving
                    if (event.text.unicode == '\b' && newsave_name.size() > 0) { //backspace
                        newsave_name.erase(newsave_name.size()-1, 1); // delete the last character
                        SFhuman_input.setString(newsave_name);
                    }
                    else if (isAlNum(event.text.unicode)) {
                        newsave_name.push_back(static_cast<char>(event.text.unicode));
                        SFhuman_input.setString(newsave_name);
                    }
                    else if (event.text.unicode == '\r'){
                        saveGame(word, word_hidden, used, newsave_name);
                        mode = 1;
                    }
                    SFhuman_input = center(SFhuman_input, 2.0f, 1.8f);
                }

                if (mode == 4) { // load save

                    if (event.text.unicode != '\r') {

                        if (event.text.unicode == '\b' && savefile_name.size() > 0) { //backspace
                            savefile_name.erase(savefile_name.size()-1, 1); // delete the last character
                            SFhuman_input.setString(savefile_name);
                        }
                        else if (isAlNum(event.text.unicode)) {
                            savefile_name.push_back(static_cast<char>(event.text.unicode));
                            SFhuman_input.setString(savefile_name);
                        }
                        SFhuman_input = center(SFhuman_input, 2.0f, 1.8f);
                    }

                    else {
                    // else if (savefile_name.size() >= MIN_LETTERS){

                        word = loadSaveLine(1, savefile_name);
                        word_hidden = loadSaveLine(2, savefile_name);
                        used = loadSaveLine(3, savefile_name);

                        SFtitle.setString(word);
                        SFword_hidden.setString(word_hidden);
                        SFused.setString(used);
                        fails = used.size();

                        SFtitle = center(SFtitle, 2.0f, 7.0f);
                        SFword_hidden = center(SFword_hidden, 2.0f, 2.0f);
                        mode = 1;
                    }


                }

                if (mode == 1) { //game loop

                    // SFword_hidden = center(SFword_hidden, 2.0f, 2.0f);

                    if (event.text.unicode > 128)
                        letter = getPolishLetter(event.text.unicode);
                    else letter = event.text.unicode;

                    if (isLetterInWstring(word, letter) && !isLetterInWstring(word_hidden, letter)) {
                        correct_guesses++;
                        correct.play();
                    }

                    word_hidden = fillWord(word, word_hidden, letter);
                    SFword_hidden.setString(word_hidden);

                    //fail
                    if (!isLetterInWstring(word,letter) && !isLetterInWstring(used,letter) && isLetterAllowed(event.text.unicode)) {
                            used+=letter;
                            fails++;
                            incorrect.play();
                            if (fails == 10) { // lose condition
                                mode = 6;
                            }
                    }

                    SFused.setString(used);
                    SFused = center(SFused, 2.0f, 1.3f);
                    SFfails.setString(std::to_wstring((int)fails)+L"/10");
                    SFfails = center(SFfails, 2.0f, 1.4f);

                    // check for the win
                    if (playerHasWon(word_hidden)) {
                        mode = 2;
                    }
                }
            }
            window.clear(BackgroundColor);

            if (mode == 0) { //menu
                SFkeybinds = applyStyle(SFkeybinds, font, 40, FontColor, CONFIG.FNT_MULTIPLIER);
                SFkeybinds = center(SFkeybinds, 2.0f, 1.4f);
                window.draw(SFtitle);
                window.draw(SFmain_menu);
                window.draw(SFkeybinds);
                SFkeybinds = applyStyle(SFkeybinds, font, 16, FontColor, CONFIG.FNT_MULTIPLIER);
                SFkeybinds = center(SFkeybinds, 5.0f, 1.1f);
                // window.draw(begin_game_button);
                // window.draw(end_game_button);
                // window.draw(new_game);
                // window.draw(load_save);
            }
            if (mode == 1) { //game
                window.draw(SFword_hidden);
                window.draw(used_letters);
                window.draw(SFused);
                window.draw(SFfails);
            }
            if (mode == 2) { //win
                SFtitle.setString(L"Congratulations!");
                SFtitle = center(SFtitle, 2.0f, 7.0f);
                window.draw(SFtitle);

                SFword_reveal.setString(L"The word was: "+word);
                SFword_reveal = center(SFword_reveal, 2.0f, 3.6f);
                window.draw(SFword_reveal);

                SFendcorrect_guesses.setString("Correct guesses: " + std::to_string((int)correct_guesses));
                SFendcorrect_guesses = center(SFendcorrect_guesses, 2.0f, 2.6f);
                window.draw(SFendcorrect_guesses);

                SFendfails.setString("Incorrect guesses: " + std::to_string((int)fails));
                SFendfails = center(SFendfails, 2.0f, 2.3f);
                window.draw(SFendfails);

                SFcorrect_incorrect_ratio.setString("Ratio: " + std::to_string((int)(correct_guesses/(correct_guesses+fails)*100))+"%");
                SFcorrect_incorrect_ratio = center(SFcorrect_incorrect_ratio, 2.0f, 1.9f);
                window.draw(SFcorrect_incorrect_ratio);


            }
            if (mode == 3) { // human input
                SFinput_prompt = center(SFinput_prompt, 2.0f, 2.0f);
                SFtitle = center(SFtitle, 2.0f, 7.0f);
                window.draw(SFinput_prompt);
                window.draw(SFhuman_input);
                window.draw(SFtitle);
            }
            if (mode == 4) { // loading a save
                SFtitle.setString("Load a game");
                SFtitle = center(SFtitle, 2.0f, 7.0f);
                SFinput_prompt.setString("Input the name of a save to load");
                SFinput_prompt = center(SFinput_prompt, 2.0f, 2.0f);
                window.draw(SFhuman_input);
                window.draw(SFtitle);
                window.draw(SFinput_prompt);
            }
            if (mode == 5) { // saving the game
                SFtitle.setString("Save the game");
                SFtitle = center(SFtitle, 2.0f, 7.0f);
                SFinput_prompt.setString("Input the name of a new save");
                SFinput_prompt = center(SFinput_prompt, 2.0f, 2.0f);
                window.draw(SFinput_prompt);
                window.draw(SFtitle);
                window.draw(SFhuman_input);
            }
            if (mode == 6) {
                SFtitle.setString("You've lost!");
                SFtitle = center(SFtitle, 2.0f, 7.0f);
                window.draw(SFtitle);

                SFword_reveal.setString(L"The word was: "+word);
                SFword_reveal = center(SFword_reveal, 2.0f, 3.6f);
                window.draw(SFword_reveal);

                SFendcorrect_guesses.setString("Correct guesses: " + std::to_string((int)correct_guesses));
                SFendcorrect_guesses = center(SFendcorrect_guesses, 2.0f, 2.6f);
                window.draw(SFendcorrect_guesses);

                SFendfails.setString("Incorrect guesses: " + std::to_string((int)fails));
                SFendfails = center(SFendfails, 2.0f, 2.3f);
                window.draw(SFendfails);

                SFcorrect_incorrect_ratio.setString("Ratio: " + std::to_string((int)(correct_guesses/(correct_guesses+fails)*100))+"%");
                SFcorrect_incorrect_ratio = center(SFcorrect_incorrect_ratio, 2.0f, 1.9f);
                window.draw(SFcorrect_incorrect_ratio);
            }

            if (mode == 7) {
                SFtitle.setString("Hangman");
                SFtitle = center(SFtitle, 2.0f, 7.0f);
                window.draw(SFtitle);
                window.draw(SFgoback_prompt);
                window.draw(SFauthors);
                window.draw(SFyear);
                window.draw(SFlicense);
                window.draw(SFinfo);

            }
            if (CONFIG.KB_PROMPTS && mode != 0 && mode != 7) {
                window.draw(SFkeybinds);
            }
            window.display();
        }
    }
    return EXIT_SUCCESS;
}
