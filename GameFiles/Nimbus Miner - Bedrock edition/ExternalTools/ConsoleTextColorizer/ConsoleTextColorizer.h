#pragma once

#include <string>

#include <Windows.h>

#pragma region WindowsTextColorEnum

/// <summary>
/// Represents available text colors for the Windows console. </summary>
enum WindowsTextColorEnum
{
    Black,
    Blue,
    Green,
    Aqua,
    Red,
    Purple,
    Yellow,
    White,
    Gray,
    LightBlue,
    LightGreen,
    LightAqua,
    LightRed,
    LightPurple,
    LightYellow,
    BrightWhite,
};

inline std::string WindowsTextColorEnumToString(const WindowsTextColorEnum p_color)
{
    switch (p_color)
    {
        case WindowsTextColorEnum::Black: return "Black";
        case WindowsTextColorEnum::Blue: return "Blue";
        case WindowsTextColorEnum::Green: return "Green";
        case WindowsTextColorEnum::Aqua: return "Aqua";
        case WindowsTextColorEnum::Red: return "Red";
        case WindowsTextColorEnum::Purple: return "Purple";
        case WindowsTextColorEnum::Yellow: return "Yellow";
        case WindowsTextColorEnum::White: return "White";
        case WindowsTextColorEnum::Gray: return "Gray";
        case WindowsTextColorEnum::LightBlue: return "LightBlue";
        case WindowsTextColorEnum::LightGreen: return "LightGreen";
        case WindowsTextColorEnum::LightAqua: return "LightAqua";
        case WindowsTextColorEnum::LightRed: return "LightRed";
        case WindowsTextColorEnum::LightPurple: return "LightPurple";
        case WindowsTextColorEnum::LightYellow: return "LightYellow";
        case WindowsTextColorEnum::BrightWhite: return "BrightWhite";
	    	
        default: return "Unknown";
    }
}

#pragma endregion WindowsTextColorEnum

/// <summary>
/// Utility class for changing the console text color on Windows. </summary>
class ConsoleTextColorizer
{

public:
    
    static constexpr WindowsTextColorEnum DEFAULT_COLOR = WindowsTextColorEnum::White;
    
private:

    // NOTE : Because the type 'WORD' is just an 'unsigned short' we can "forward declare it" by replacing WORD by an unsigned short
    static unsigned short _previousConsoleAttributes;

public:

    /// <summary>
    /// Sets the text color of the Windows console to the specified color.
    /// <para> This method stores the current color before applying the new one, allowing an undo. </para> </summary>
    /// 
    /// <param name = "p_newTextColor"> The new text color to apply.</param>
    static void SetConsoleTextColor(WindowsTextColorEnum p_newTextColor);

    /// <summary>
    /// Resets the console text color to the default color (White). </summary>
    static void ResetConsoleTextColorToDefault();

    /// <summary>
    /// Reverts the console text color to the color used before the last call to SetConsoleTextColor.
    /// 
    /// <para> Calling this method multiple times has no additional effect: 
    /// only the most recent pre-change state is saved and restored. </para> </summary>
    static void UndoConsoleTextColor();
};