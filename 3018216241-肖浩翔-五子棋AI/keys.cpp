#include "keys.h"

int keyBoradHit()
{
    int key_value;
#ifdef __linux__
    struct termios new_config;
    struct termios old_config;

    tcgetattr(0, &old_config);
    new_config = old_config;
    new_config.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new_config);

    key_value = getchar();

    tcsetattr(0, TCSANOW, &old_config);
#else
    for (;;)
    {
        if (_kbhit())
        {
            key_value = _getch();
            break;
        }
    }
#endif // __linux__
    return key_value;
}