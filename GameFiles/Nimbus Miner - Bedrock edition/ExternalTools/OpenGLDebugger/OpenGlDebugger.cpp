#include "OpenGlDebugger.h"

#include <ostream>

void OpenGlDebugger::PrintOpenGlErrors(
    const GLenum p_source,
    const GLenum p_type,
    const GLuint p_id,
    const GLenum p_severity,
    const GLsizei p_length,
    const GLchar* p_message,
    const void* p_userParam)
{
    std::string printedMessage = std::string("OpenGL report : \n") +
        "- Severity : " + GetSeverityString(p_severity) + "\n" +
        "- Type     : " + GetTypeString(p_type) + "\n"
        "- Source   : " + GetSourceString(p_source) + "\n" +
        "\nMessage :\n" + p_message;

    switch (p_severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        MessageDebugger::PrintCustomMessage(LightPurple, printedMessage, "Unknown", -1);
        break;

    case GL_DEBUG_SEVERITY_LOW:
        MessageDebugger::PrintWarning(printedMessage, "Unknown", -1, true);
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        MessageDebugger::PrintWarning(printedMessage, "Unknown", -1, true);
        break;

    case GL_DEBUG_SEVERITY_HIGH:
        MessageDebugger::PrintError(printedMessage, "Unknown", -1, true);
        break;


    default:
        PRINT_ERROR_RUNTIME(false, "The given 'p_severity' is not planned in the switch.")
        break;
    }
}

#pragma region // -=- Sub-methods -=- //

std::string OpenGlDebugger::GetSourceString(const GLenum p_source)
{
    switch (p_source)
    {
    case GL_DEBUG_SOURCE_API:             return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY:     return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION:     return "Application";
    case GL_DEBUG_SOURCE_OTHER:           return "Other";

    default: return "Unknown";
    }
}

std::string OpenGlDebugger::GetTypeString(const GLenum p_type)
{
    switch (p_type)
    {
    case GL_DEBUG_TYPE_ERROR:               return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated Behaviour";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "Undefined Behaviour";
    case GL_DEBUG_TYPE_PORTABILITY:         return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE:         return "Performance";
    case GL_DEBUG_TYPE_OTHER:               return "Other";

    default: return "Unknown";
    }
}

std::string OpenGlDebugger::GetSeverityString(const GLenum p_severity)
{
    switch (p_severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION: return "Notification";
    case GL_DEBUG_SEVERITY_LOW:          return "Low";
    case GL_DEBUG_SEVERITY_MEDIUM:       return "Medium";
    case GL_DEBUG_SEVERITY_HIGH:         return "High";

    default: return "Unknown";
    }
}

#pragma endregion