#include "ConsoleTextColorizer.h"

#include <windows.h>

WORD ConsoleTextColorizer::_previousConsoleAttributes = ConsoleTextColorizer::DEFAULT_COLOR;

void ConsoleTextColorizer::ResetConsoleTextColorToDefault()
{
	SetConsoleTextColor(DEFAULT_COLOR);
}

void ConsoleTextColorizer::SetConsoleTextColor(const WindowsTextColorEnum p_newTextColor)
{
	const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Saving the current console attributes (contains the text color)
	CONSOLE_SCREEN_BUFFER_INFO consoleInformation;

	if (GetConsoleScreenBufferInfo(consoleHandle, &consoleInformation))
	{
		_previousConsoleAttributes = consoleInformation.wAttributes;
	}

	SetConsoleTextAttribute(consoleHandle, p_newTextColor);
}

void ConsoleTextColorizer::UndoConsoleTextColor()
{
	const HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(consoleHandle, _previousConsoleAttributes);
}