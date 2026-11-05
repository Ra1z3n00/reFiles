#pragma once

#ifndef NOMINMAX
    #define NOMINMAX
#endif

#ifdef _WIN32
    #include <windows.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <type_traits>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <regex>
#include <cmath>
using namespace std;

// --- DEFINIZIONI COLORI E MACRO ANSI ---
#define RESET               "\033[0m"
#define BOLD                "\033[1m"
#define DIM                 "\033[2m"
#define UNDERLINE           "\033[4m"
#define LAMP                "\033[5m"
#define RED                 "\033[31m"
#define GREEN               "\033[32m"
#define YELLOW              "\033[33m"
#define BLUE                "\033[34m"
#define MAGENTA             "\033[35m"
#define CYAN                "\033[36m"
#define CLEAR_LINE          "\033[2K"
#define HIDE_CURSOR         "\033[?25l"
#define SHOW_CURSOR         "\033[?25h"
#define SAVE_CURSOR         "\033[s"
#define RESTORE_CURSOR      "\033[u"
#define EXT_TXT_RGB(r,g,b)  "\033[38;2;" #r ";" #g ";" #b "m"

// --- PROTOTIPI BASE ---
void clearBuffer();
void hide_Input(bool value);
void hide_Cursor(bool hide);
void PAUSE(const string& testo);

// --- GESTIONE GETCH CROSS-PLATFORM ---
#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    int getch(void);
#endif

 
// --- TEMPLATE UTILITY PER UTF-8 ---
template<typename tipo>
int maxStringUTF8(const vector<tipo>& menu) {
    int maxLen = 0;
    for (const auto& item : menu) {
        int realChars = 0;
        size_t i = 0;
        while (i < item.testo.length()) {
            unsigned char c = item.testo[i];
            int len = (c >= 0xC0 && c < 0xE0) ? 2 : (c >= 0xE0 && c < 0xF0) ? 3 : (c >= 0xF0) ? 4 : 1;
            realChars++;
            i += len;
        }
        if (realChars > maxLen) {
            maxLen = realChars;
        }
    }
    return maxLen;
}