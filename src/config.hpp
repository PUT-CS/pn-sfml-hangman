#ifndef config
#define config
#include <iostream>

typedef struct{
	int BG_R;
	int BG_G;
	int BG_B;
	int FNT_R;
	int FNT_G;
	int FNT_B;
	float FNT_MULTIPLIER;
	int KB_PROMPTS;
	std::wstring FONT_FILE;
	std::wstring WINDOW_NAME;
	std::wstring WORDLIST;
	int MUSIC_ON;
	std::wstring MUSIC_NAME;
	float MUSIC_VOL_MULTIPLIER;
	float SOUNDS_VOL_MULTIPLIER;
}Config;

Config getConfig();

#endif
