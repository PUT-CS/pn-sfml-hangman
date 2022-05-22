#include "config.hpp"
#include <fstream>
#include <iostream>

Config getConfig(){
	Config CONFIG;
	std::wifstream file("config.hangman");
	std::wstring dump;

	file >> dump;
	file >> CONFIG.BG_R;
	file >> dump;
	file >> CONFIG.BG_G;
	file >> dump;
	file >> CONFIG.BG_B;
	file >> dump;
	file >> CONFIG.FNT_R;
	file >> dump;
	file >> CONFIG.FNT_G;
	file >> dump;
	file >> CONFIG.FNT_B;
	file >> dump;
	file >> CONFIG.FNT_MULTIPLIER;
	file >> dump;
	file >> CONFIG.KB_PROMPTS;
	file >> dump;
	file >> CONFIG.FONT_FILE;
	file >> dump;
	file >> CONFIG.WINDOW_NAME;
	file >> dump;
	file >> CONFIG.WORDLIST;
	file >> dump;
	file >> CONFIG.MUSIC_ON;
	file >> dump;
	file >> CONFIG.MUSIC_NAME;
	file >> dump;
	file >> CONFIG.MUSIC_VOL_MULTIPLIER;
	file >> dump;
	file >> CONFIG.SOUNDS_VOL_MULTIPLIER;

	file.close();
	return CONFIG;
}
