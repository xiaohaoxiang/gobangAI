#ifndef KEYS_H
#define KEYS_H

#ifdef __linux__
#include <cstdio>
#include <termios.h>
#else
#include <conio.h>
#endif // __linux__

int keyBoradHit();

#endif // KEYS_H