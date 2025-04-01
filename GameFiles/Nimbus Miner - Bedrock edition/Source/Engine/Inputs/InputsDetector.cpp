#include "InputsDetector.h"

#include "MessageDebugger/MessageDebugger.h"

// Defining static variables
GLFWwindow* InputsDetector::_currentWindow = nullptr;
std::unordered_map<int, std::vector<InputCallback>> InputsDetector::_keyCallbacks;

void InputsDetector::Init(GLFWwindow* p_window)
{
    _currentWindow = p_window;
}

void InputsDetector::SetCurrentWindow(GLFWwindow* p_window)
{
    _currentWindow = p_window;
}

void InputsDetector::AddCallback(const int p_key, const InputCallback& p_callback)
{
    _keyCallbacks[p_key].push_back(p_callback);
}

void InputsDetector::ProcessInputs()
{
    if (_currentWindow == nullptr)
        PRINT_ERROR_RUNTIME(true, "The variable _currentWindow is equal to nullptr, please set it with Init() or SetCurrentWindow().")

    for (const std::pair<const int, std::vector<InputCallback>>& inputCallback : _keyCallbacks)
    {
        // NOTE :
        // inputCallback.first  = the input
        // inputCallback.second = the callback (the function)
        
        if (glfwGetKey(_currentWindow, inputCallback.first) == GLFW_PRESS)
        {
            for (const InputCallback& callback : inputCallback.second)
            {
                callback();
            }
        }
    }
}