#include <windows.h>

#include "ConsoleHandler.h"

/**
 * Private constructor.
 * 
 */
ConsoleHandler::ConsoleHandler()
{
    m_hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
}

/**
 * Static getter for the instance of this singleton class.
 *
 * \return Instance of the singleton class.
 */
ConsoleHandler* ConsoleHandler::getInstance()
{
    static ConsoleHandler instance;
    return &instance;
}

/**
 * Clears the console.
 * 
 */
void ConsoleHandler::clearScreen()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    SMALL_RECT scrollRect{};
    COORD scrollTarget{};
    CHAR_INFO fill{};

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(m_hStdout, &csbi))
        return;


    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = csbi.dwSize.X;
    scrollRect.Bottom = csbi.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    scrollTarget.X = 0;
    scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = csbi.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(m_hStdout, &scrollRect, NULL, scrollTarget, &fill);

    // Move the cursor to the top left corner too.
    csbi.dwCursorPosition.X = 0;
    csbi.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(m_hStdout, csbi.dwCursorPosition);
}