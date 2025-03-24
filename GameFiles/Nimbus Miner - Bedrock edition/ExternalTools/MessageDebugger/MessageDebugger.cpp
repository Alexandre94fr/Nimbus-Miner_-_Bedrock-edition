#include "MessageDebugger.h"

#include <iostream>
#include <ostream>

#include "../ConsoleTextColorizer/ConsoleTextColorizer.h"
#include "../RuntimeLogger/RuntimeLogger.h"

#pragma region // -=- MessageDebugger test method -=- //

void MessageDebugger::TestMessageDebugger(const TestOptionsEnum p_testOptionsEnum)
{
    if (p_testOptionsEnum == TestOptionsEnum::NoTests)
        return;
    
    // In case the console text color was already modified by a developer
    ConsoleTextColorizer::ResetConsoleTextColorToDefault();
    
    PrintCustomMessage(Aqua, "\n===========================================");
    PrintCustomMessage(LightAqua, "  Beginning of the MessageDebugger test :");
    PrintCustomMessage(Aqua, "===========================================\n\n");

    #pragma region // - Selecting and doing the tests - //

    switch (p_testOptionsEnum)
    {
    case NoTests:
        // The code should not ever get here (because there is a check before)
        break;
        
        
    case (OnlyPrintingMessageTest):
        TestPrintingMethods();
        break;
        
        
    case (OnlyMacrosTest):
        TestMacros();
        break;

    case (OnlyWarningMacrosTest):
        TestWarningMacros();
        break;
        
    case (OnlyErrorMacrosTest):
        TestErrorMacros();
        break;

        
    case (OnlyPrintingMessageAndWarningMacrosTest):
        TestPrintingMethods();
        TestWarningMacros();
        break;

    case (OnlyPrintingMessageAndErrorMacrosTest):
        TestPrintingMethods();
        TestErrorMacros();
        break;

        
    case (AllTestsExceptErrorMacrosTest):
        TestPrintingMethods();
        TestMessageMacros();
        TestWarningMacros();
        break;
        
    case (AllTests):
        TestPrintingMethods();
        TestMacros();
        break;

        
    default:
        PRINT_ERROR_RUNTIME(false, "The given p_testOptionsEnum : '" + ToString(p_testOptionsEnum) + "' is not planned in the switch.")
        break;
    }

    #pragma endregion
    
    PrintCustomMessage(Aqua, "\n=====================================");
    PrintCustomMessage(LightAqua, "  End of the MessageDebugger test :");
    PrintCustomMessage(Aqua, "=====================================\n");
}

#pragma region // - Sub-testing methods - //

void MessageDebugger::TestPrintingMethods()
{
    PrintCustomMessage(Purple, "- - - - - - - - - - - - - - - - - - - - -");
    PrintCustomMessage(LightPurple, "Beginning of the printing methods tests :\n");

    #pragma region - Message color -

    PrintMessage("This message should be " + WindowsTextColorEnumToString(MESSAGE_TEXT_COLOR));
    PrintWarning("This warning should be " + WindowsTextColorEnumToString(WARNING_TEXT_COLOR) + ", and should have a prefix like for example 'WARNING ! '");
    PrintError("This error should be " + WindowsTextColorEnumToString(ERROR_TEXT_COLOR) + ", and should have a prefix like for example 'ERROR ! '", false);
    PrintCustomMessage(Green, "This custom message should be Green");

    #pragma endregion

    #pragma region - Prefix only color -

    std::cout << "\n";

    PrintWarning(
        "ONLY this warning prefix should be " + WindowsTextColorEnumToString(WARNING_TEXT_COLOR) +
        ", the rest of the warning should be " + WindowsTextColorEnumToString(ConsoleTextColorizer::DEFAULT_COLOR),
        true
    );
    PrintError(
        "ONLY this error prefix should be " + WindowsTextColorEnumToString(ERROR_TEXT_COLOR) +
        ", the rest of the error should be " + WindowsTextColorEnumToString(ConsoleTextColorizer::DEFAULT_COLOR),
        true
    );

    #pragma endregion

    #pragma region - Message color after SetConsoleTextColor method use -

    std::cout << "\n";
    ConsoleTextColorizer::SetConsoleTextColor(Red);

    PrintMessage("This message should be " + WindowsTextColorEnumToString(MESSAGE_TEXT_COLOR) + " not Red");
    std::cout << "But this one should be Red\n";

    PrintWarning(
        "ONLY this warning prefix should be " + WindowsTextColorEnumToString(WARNING_TEXT_COLOR) +
        ", the rest of the warning should be Red",
        true
    );

    #pragma endregion

    ConsoleTextColorizer::ResetConsoleTextColorToDefault();

    PrintCustomMessage(LightPurple, "\nEnd of the printing methods tests :");
    PrintCustomMessage(Purple, "- - - - - - - - - - - - - - - - - - -\n");
}

void MessageDebugger::TestMacros()
{
    PrintCustomMessage(Purple, "- - - - - - - - - - - - - - - -");
    PrintCustomMessage(LightPurple, "Beginning of the macros tests :\n");

    TestMessageMacros();
    TestWarningMacros();
    TestErrorMacros();
    
    PrintCustomMessage(LightPurple, "End of the macros tests :");
    PrintCustomMessage(Purple, "- - - - - - - - - - - - -");
}

void MessageDebugger::TestMessageMacros()
{
    PrintCustomMessage(LightPurple, "Beginning of the message macros tests :\n");
    
    PRINT_MESSAGE_RUNTIME("I'm a message called by a macro")

    PRINT_CUSTOM_MESSAGE_RUNTIME(Green, "I'm a custom message called by a macro (should be Green)")

    PrintCustomMessage(LightPurple, "End of the message macros tests\n");
}

void MessageDebugger::TestWarningMacros()
{
    PrintCustomMessage(LightPurple, "Beginning of the warning macros tests :\n");
    
    PRINT_WARNING_RUNTIME(false, "I'm a warning called by a macro")
    PRINT_WARNING_RUNTIME(true, "I'm a warning called by a macro, with only the prefix colored")
    
    PrintCustomMessage(LightPurple, "End of the warning macros tests\n");
}

void MessageDebugger::TestErrorMacros()
{
    PrintCustomMessage(LightPurple, "Beginning of the error macros tests :\n");
    
    PRINT_ERROR_RUNTIME(false, "I'm an error called by a macro")
    PRINT_ERROR_RUNTIME(true, "I'm an error called by a macro, with only the prefix colored")

    PrintCustomMessage(LightPurple, "Beginning of the error macros tests\n");
}

#pragma endregion

#pragma endregion

#pragma region // -=- Printing methods -=- //

#pragma region // - Message - //

void MessageDebugger::PrintMessage(const std::string& p_message)
{
    ConsoleTextColorizer::SetConsoleTextColor(MESSAGE_TEXT_COLOR);
    
    std::cout << p_message << "\n";
    
    ConsoleTextColorizer::UndoConsoleTextColor();
}

void MessageDebugger::PrintMessage(const std::string& p_message, const char* p_filePath, const int p_line)
{
    PrintMessage(p_message);

    ConsoleTextColorizer::SetConsoleTextColor(MESSAGE_TEXT_COLOR);
    
    std::cout << "\nMessage triggered at '" << p_filePath << "' at line : " << p_line << "\n\n";

    ConsoleTextColorizer::UndoConsoleTextColor();
    
    RuntimeLogger::Log("MESSAGE : " + p_message, p_filePath, p_line);
}

#pragma endregion

#pragma region // - Warning - //

void MessageDebugger::PrintWarning(const std::string& p_message, const bool p_isOnlyPrefixColored)
{
    ConsoleTextColorizer::SetConsoleTextColor(WARNING_TEXT_COLOR);

    std::cout << WARNING_TEXT_PREFIX;
    
    if (p_isOnlyPrefixColored)
    {
        ConsoleTextColorizer::UndoConsoleTextColor();
        std::cout << p_message << "\n";
    }
    else
    {
        std::cout << p_message << "\n";
        ConsoleTextColorizer::UndoConsoleTextColor();
    }
}

void MessageDebugger::PrintWarning(const std::string& p_message, const char* p_filePath, const int p_line, const bool p_isOnlyPrefixColored)
{
    PrintWarning(p_message, p_isOnlyPrefixColored);

    ConsoleTextColorizer::SetConsoleTextColor(WARNING_TEXT_COLOR);
    
    std::cout << "\nWarning triggered at '" << p_filePath << "' at line : " << p_line << "\n\n";

    RuntimeLogger::Log(WARNING_TEXT_PREFIX + p_message, p_filePath, p_line);
    
    ConsoleTextColorizer::ResetConsoleTextColorToDefault();
}
#pragma endregion

#pragma region // - Error - //

void MessageDebugger::PrintError(const std::string& p_message, const bool p_isOnlyPrefixColored)
{
    ConsoleTextColorizer::SetConsoleTextColor(ERROR_TEXT_COLOR);
    
    std::cout << ERROR_TEXT_PREFIX;
    
    if (p_isOnlyPrefixColored)
    {
        ConsoleTextColorizer::UndoConsoleTextColor();
        std::cout << p_message << "\n";
    }
    else
    {
        std::cout << p_message << "\n";
        ConsoleTextColorizer::UndoConsoleTextColor();
    }
}

void MessageDebugger::PrintError(const std::string& p_message, const char* p_filePath, const int p_line, const bool p_isOnlyPrefixColored)
{
    PrintError(p_message, p_isOnlyPrefixColored);

    ConsoleTextColorizer::SetConsoleTextColor(ERROR_TEXT_COLOR);
    
    std::cout << "\nError triggered at '" << p_filePath << "' at line : " << p_line << "\n\n";
    
    RuntimeLogger::Log(ERROR_TEXT_PREFIX + p_message, p_filePath, p_line);

    ConsoleTextColorizer::ResetConsoleTextColorToDefault();
}
#pragma endregion

#pragma region // - Custom message - //

void MessageDebugger::PrintCustomMessage(const WindowsTextColorEnum p_messageColor, const std::string& p_message)
{
    ConsoleTextColorizer::SetConsoleTextColor(p_messageColor);
    
    std::cout << p_message << "\n";
    
    ConsoleTextColorizer::UndoConsoleTextColor();
}

void MessageDebugger::PrintCustomMessage(const WindowsTextColorEnum p_messageColor, const std::string& p_message, const char* p_filePath, const int p_line)
{
    PrintCustomMessage(p_messageColor, p_message);

    ConsoleTextColorizer::SetConsoleTextColor(p_messageColor);
    
    std::cout << "\nCustom message triggered at '" << p_filePath << "' at line : " << p_line << "\n\n";

    ConsoleTextColorizer::UndoConsoleTextColor();
    
    RuntimeLogger::Log("CUSTOM MESSAGE : " + p_message, p_filePath, p_line);
}

#pragma endregion

#pragma endregion