#ifndef SIGN_UP_H
#define SIGN_UP_H

#ifdef __linux__
    #define CLEAR "clear"
#elif _WIN64 || _WIN32
    #define CLEAR "cls"
#elif __APPLE__
    #define CLEAR "clear"
#elif __unix__
    #define CLEAR "clear"
#endif

int char_check(char input[]);
int pwd_check(char input[]);
void sign_up();

#endif 

