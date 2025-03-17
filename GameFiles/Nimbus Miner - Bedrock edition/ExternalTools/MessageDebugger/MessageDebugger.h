#pragma once

#include "../ConsoleTextColorizer/ConsoleTextColorizer.h"

// Forward declarations
enum WindowsTextColorEnum : int;
class ConsoleTextColorizer;

#pragma region // -=- Enums -=- // 

enum TestOptionsEnum
{
    NoTests,
    
    OnlyPrintingMessageTest,
    
    OnlyMacrosTest,
    OnlyWarningMacrosTest,
    OnlyErrorMacrosTest,

    OnlyPrintingMessageAndWarningMacrosTest,
    OnlyPrintingMessageAndErrorMacrosTest,
    
    AllTests
};

inline std::string ToString(const TestOptionsEnum p_testOptionsEnum)
{
    switch (p_testOptionsEnum)
    {
    case TestOptionsEnum::NoTests: return "NoTests";
        
    case TestOptionsEnum::OnlyPrintingMessageTest: return "OnlyPrintingMessageTest";
        
    case TestOptionsEnum::OnlyMacrosTest: return "OnlyMacrosTest";
    case TestOptionsEnum::OnlyWarningMacrosTest: return "OnlyWarningMacrosTest";
    case TestOptionsEnum::OnlyErrorMacrosTest: return "OnlyErrorMacrosTest";

    case OnlyPrintingMessageAndWarningMacrosTest: return "OnlyPrintingMessageAndWarningMacrosTest";
    case OnlyPrintingMessageAndErrorMacrosTest: return "OnlyPrintingMessageAndErrorMacrosTest";
        
    case TestOptionsEnum::AllTests: return "AllTests";
	    	
    default: return "Unknown";
    }
}

#pragma endregion

/// <summary>
/// Utility class for printing formatted messages to the console with different severity levels (Message, Warning, Error).
/// 
/// <para> 
/// Also provides macros for printing Warnings and Errors with file and line information during compilation, 
/// will also be shown as warning and error by your IDE (tested for Visual Studio and JetBrain Rider). </para>
/// 
/// <para>
/// Beware ! If you are on Visual Studio, the error macros will generate an error popup (I can't control it), 
/// just press the 'No' button to see the error. </para> </summary>
class MessageDebugger
{

private:

    // NOTE : If you want you can modify those constants, the code will adapt

    static constexpr int TEXT_PREFIX_MAXIMAL_LENGHT = 50;

    static constexpr char WARNING_TEXT_PREFIX[TEXT_PREFIX_MAXIMAL_LENGHT] = "WARNING ! ";
    static constexpr char ERROR_TEXT_PREFIX[TEXT_PREFIX_MAXIMAL_LENGHT] = "ERROR ! ";
    
    static constexpr WindowsTextColorEnum MESSAGE_TEXT_COLOR = WindowsTextColorEnum::White;
    static constexpr WindowsTextColorEnum WARNING_TEXT_COLOR = WindowsTextColorEnum::Yellow;
    static constexpr WindowsTextColorEnum ERROR_TEXT_COLOR = WindowsTextColorEnum::Red;
    
public:

    #pragma region // -=- Methods -=- //

    #pragma region // - Class testing methods - //

    /// <summary>
    /// Prints sample messages using printing methods AND macros, 
    /// for testing the '<see cref="MessageDebugger"/>' system. 
    /// 
    /// <para>
    /// Beware ! Because we are testing macros, and more precisely the '<see cref="PRINT_ERROR_COMPILATION"/>' one, 
    /// it will generate real errors, so don't be surprised. </para> </summary>
    static void TestMessageDebugger(TestOptionsEnum p_testOptionsEnum);

private:

    /// <summary>
    /// Will test if the printing methods are working. </summary>
    static void TestPrintingMethods();

    /// <summary>
    /// Will test if the macros are working.
    /// 
    /// <para>
    /// Beware ! Because we are testing macros, and more precisely the '<see cref="PRINT_ERROR_COMPILATION"/>' one, 
    /// it will generate real errors, so don't be surprised. </para> </summary>
    static void TestMacros();

    static void TestWarningMacros();
    static void TestErrorMacros();

public:
    
    #pragma endregion

    #pragma region // - Printing methods (without File and Line) - //

    /// <summary>
    /// Prints a standard message in the default message color. </summary>
    static void PrintMessage(const std::string& p_message);

    /// <summary>
    /// Prints a warning message with optional prefix-only coloring. 
    /// 
    /// <para>
    /// Beware ! It will not show it as a warning in your IDE, use the '<see cref="PRINT_WARNING_COMPILATION"/>' macro instead. </para> </summary>
    static void PrintWarning(const std::string& p_message, bool p_isOnlyPrefixColored = false);

    /// <summary>
    /// Prints an error message with optional prefix-only coloring.
    ///  
    /// <para>
    /// Beware ! It will not show it as an error in your IDE, use the '<see cref="PRINT_ERROR_COMPILATION"/>' macro instead. </para> </summary>
    static void PrintError(const std::string& p_message, const bool p_isOnlyPrefixColored = false);
    
    /// <summary>
    /// Prints a custom message with the specified console text color. </summary>
    static void PrintCustomMessage(const std::string& p_message, const WindowsTextColorEnum p_messageColor);
    
    #pragma endregion

    #pragma region // - Warning and Error methods (with File and Line) - //

    /// <summary>
    /// Prints a warning with file and line information, and optional prefix-only coloring. 
    /// 
    /// <para> 
    /// You can use the '<see cref="PRINT_WARNING_COMPILATION"/>' or the '<see cref="PRINT_WARNING_RUNTIME"/>' macro 
    /// to not have to put the 'p_filePath' and 'p_line' parameters manually. </para> </summary>
    static void PrintWarning(const std::string& p_message, const char* p_filePath, const int p_line, const bool p_isOnlyPrefixColored = false);

    /// <summary>
    /// Prints an error with file and line information, and optional prefix-only coloring.
    /// 
    /// <para> 
    /// You can use the '<see cref="PRINT_ERROR_COMPILATION"/>' or the '<see cref="PRINT_ERROR_RUNTIME"/>' macro 
    /// to not have to put the 'p_filePath' and 'p_line' parameters manually. </para> 
    /// 
    /// <para>
    /// Beware ! If you are on Visual Studio, the error macro '<see cref="PRINT_ERROR_COMPILATION"/>'
    /// will generate an error popup (I can't control it), just press the 'No' button to see the error. </para> </summary>
    static void PrintError(const std::string& p_message, const char* p_filePath, const int p_line, const bool p_isOnlyPrefixColored = false);

    #pragma endregion

    #pragma endregion
};

// Macros to use to get automatically the file and the line of call

#pragma region // -=- Macros -=- //

#pragma region // - Macro helper - //

/// <summary>
/// Internal macro used to convert a macro value to a string. 
/// 
/// <para> 
/// Example : STRINGIZE_INTERNAL(__LINE__) -> "__LINE__" </para> </summary>
#define STRINGIZE_INTERNAL(value) #value

/// <summary>
/// Converts a macro argument to a string.
/// 
/// <para> 
/// Example : STRINGIZE(__LINE__) -> 42 -> "42" </para> </summary>
#define STRINGIZE(value) STRINGIZE_INTERNAL(value)

#pragma endregion

#pragma region // - Usable macros - //

/// <summary>
/// Prints a warning message <b> at compilation </b> with prefix-only coloring and with file and line info. 
/// The message <b> is </b> shown in the error list and build output, 
/// so you click on the button and be transported directly to the concerned line. 
/// 
/// <para>
/// Using the compilation one means you will not be able to pass dynamic data as 'p_message'. 
/// Use the '<see cref="PRINT_WARNING_RUNTIME"/>' one instead. </para> 
/// 
/// <para>
/// Example : <code> PRINT_WARNING_COMPILATION(true, "The warning should not work" + "without giving problem") </code> </para> </summary>
#define PRINT_WARNING_COMPILATION(p_isOnlyPrefixColored, p_message) \
    MessageDebugger::PrintWarning(p_message, __FILE__, __LINE__, p_isOnlyPrefixColored); \
    __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) "): warning: " p_message))

// TODO : Essayer de faire une macro qui converti un std::string en const char[]  

/// <summary>
/// Prints a warning message <b> at runtime </b> with prefix-only coloring and with file and line info. 
/// The message <b> is NOT </b> shown in the error list and build output. But </summary>
#define PRINT_WARNING_RUNTIME(p_isOnlyPrefixColored, p_message) \
    MessageDebugger::PrintWarning(p_message, __FILE__, __LINE__, p_isOnlyPrefixColored); \

/// <summary>
/// Prints an error message <b> at compilation </b> with prefix-only coloring and with file and line info. 
/// The message <b> is </b> shown in the error list and build output, 
/// so you click on the button and be transported directly to the concerned line.
/// 
/// <para>
/// Using the compilation one means you will not be able to pass dynamic data as 'p_message'. 
/// Use the '<see cref="PRINT_ERROR_RUNTIME"/>' one instead. </para> 
/// 
/// <para>
/// Example : <code> PRINT_ERROR_COMPILATION(true, "The warning should not work" + "without giving problem") </code> </para>
/// 
/// <para>
/// Beware ! If you are on Visual Studio, the error macros will generate an error popup (I can't control it), 
/// just press the 'No' button to see the error. </para> </summary>
#define PRINT_ERROR_COMPILATION(p_isOnlyPrefixColored, p_message) \
    MessageDebugger::PrintError(p_message, __FILE__, __LINE__, p_isOnlyPrefixColored); \
    __pragma(message(__FILE__ "(" STRINGIZE(__LINE__) "): error: " p_message)) 

/// <summary>
/// Prints an error message <b> at runtime </b> with prefix-only coloring and with file and line info. 
/// The message <b> is NOT </b> shown in the error list and build output. </summary>
#define PRINT_ERROR_RUNTIME(p_isOnlyPrefixColored, p_message) \
    MessageDebugger::PrintError(p_message, __FILE__, __LINE__, p_isOnlyPrefixColored); \

#pragma endregion

#pragma endregion