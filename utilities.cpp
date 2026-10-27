#include "utilities.hpp"

// Svuota il buffer di input
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

#ifdef __cplusplus
void hide_Cursor(bool hide) {
    #ifdef _WIN32
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hConsoleOutput, &structCursorInfo);
    structCursorInfo.bVisible = !hide;
    SetConsoleCursorInfo(hConsoleOutput, &structCursorInfo);
    #endif
}

void hide_Input(bool value) {
    #ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    if (value) {
        mode &= ~ENABLE_ECHO_INPUT;
    } else {
        mode |= ENABLE_ECHO_INPUT;
    }
    SetConsoleMode(hStdin, mode);
    #endif
}
#endif

#ifndef _WIN32
// Emulazione di getch per Linux / macOS
int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif