#include "MessageDebugger.h"

#include <iostream>
#include <ostream>

#include "../ConsoleTextColorizer/ConsoleTextColorizer.h"
#include "../RuntimeLogger/RuntimeLogger.h"

#pragma region // -=- MessageDebugger test method -=- //

void MessageDebugger::TestMessageDebugger(TestOptionsEnum p_testOptionsEnum)
{
    if (p_testOptionsEnum == TestOptionsEnum::NoTests)
        return;
    
    // In case the console text color was already modified by a developer
    ConsoleTextColorizer::ResetConsoleTextColorToDefault();
    
    PrintCustomMessage("\n==========================================="  , Aqua);
    PrintCustomMessage("  Beginning of the MessageDebugger test :"      , LightAqua);
    PrintCustomMessage("===========================================\n\n", Aqua);//

    #pragma region // - Selecting and doing the tests - //

    switch (p_testOptionsEnum)
    {
    case TestOptionsEnum::NoTests:
        // The code should not ever get here (because there is a check before)
        break;
        
        
    case (TestOptionsEnum::OnlyPrintingMessageTest):
        TestPrintingMethods();
        break;
        
        
    case (TestOptionsEnum::OnlyMacrosTest):
        TestMacros();
        break;

    case (TestOptionsEnum::OnlyWarningMacrosTest):
        TestWarningMacros();
        break;
        
    case (TestOptionsEnum::OnlyErrorMacrosTest):
        TestErrorMacros();
        break;


    case (TestOptionsEnum::OnlyPrintingMessageAndWarningMacrosTest):
        TestPrintingMethods();
        TestWarningMacros();
        break;

    case (TestOptionsEnum::OnlyPrintingMessageAndErrorMacrosTest):
        TestPrintingMethods();
        TestErrorMacros();
        break;
        
        
    case (TestOptionsEnum::AllTests):
        TestPrintingMethods();
        TestMacros();
        break;
        
    default:
        PRINT_ERROR_RUNTIME(false, "The given p_testOptionsEnum : '" + ToString(p_testOptionsEnum) + "' is not planned in the switch.")
        break;
    }

    #pragma endregion
    
    PrintCustomMessage("\n\n=====================================", Aqua);
    PrintCustomMessage("  End of the MessageDebugger test :"      , LightAqua);
    PrintCustomMessage("=====================================\n"  , Aqua);
}

#pragma region // - Sub-testing methods - //

void MessageDebugger::TestPrintingMethods()
{
    PrintCustomMessage("- - - - - - - - - - - - - - - - - - -", Purple);
    PrintCustomMessage("Beginning of the message color part :\n", LightPurple);

    #pragma region - Message color -

    PrintMessage("This message should be " + WindowsTextColorEnumToString(MESSAGE_TEXT_COLOR));
    PrintWarning("This warning should be " + WindowsTextColorEnumToString(WARNING_TEXT_COLOR) + ", and should have a prefix like for example 'WARNING ! '");
    PrintError("This error should be " + WindowsTextColorEnumToString(ERROR_TEXT_COLOR) + ", and should have a prefix like for example 'ERROR ! '", false);
    PrintCustomMessage("This custom message should be Green", Green);

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

    PrintCustomMessage("\nEnd of the message color part :", LightPurple);
    PrintCustomMessage("- - - - - - - - - - - - - - - -\n", Purple);
}

void MessageDebugger::TestMacros()
{
    PrintCustomMessage("- - - - - - - - - - - - - - - -", Purple);
    PrintCustomMessage("Beginning of the macro part :\n", LightPurple);

    TestWarningMacros();
    TestErrorMacros();
    
    PrintCustomMessage("End of the macro part :", LightPurple);
    PrintCustomMessage("- - - - - - - - - - - -", Purple);
}

void MessageDebugger::TestWarningMacros()
{
    PRINT_WARNING_RUNTIME(false, "I'm a warning called by a macro")
    PRINT_WARNING_RUNTIME(true, "I'm a warning called by a macro, with only the prefix colored")
}

void MessageDebugger::TestErrorMacros()
{
    PRINT_ERROR_RUNTIME(false, "I'm an error called by a macro")
    PRINT_ERROR_RUNTIME(true, "I'm an error called by a macro, with only the prefix colored")
}

#pragma endregion

#pragma endregion

#pragma region // -=- Printing methods -=- //

void MessageDebugger::PrintMessage(const std::string& p_message)
{
    ConsoleTextColorizer::SetConsoleTextColor(MESSAGE_TEXT_COLOR);
    
    std::cout << p_message << "\n";

    ConsoleTextColorizer::UndoConsoleTextColor();
}

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

void MessageDebugger::PrintCustomMessage(const std::string& p_message, const WindowsTextColorEnum p_messageColor)
{
    ConsoleTextColorizer::SetConsoleTextColor(p_messageColor);
    
    std::cout << p_message << "\n";
    
    ConsoleTextColorizer::UndoConsoleTextColor();
}
#pragma endregion