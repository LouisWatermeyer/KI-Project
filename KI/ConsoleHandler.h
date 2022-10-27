#ifndef CONSOLEHANDLER_H
#define CONSOLEHANDLER_H

#include <windows.h>

class ConsoleHandler
{
private:
    ConsoleHandler();

    HANDLE m_hStdout;

public:
    static ConsoleHandler* getInstance();

    void clearScreen();
};

#endif