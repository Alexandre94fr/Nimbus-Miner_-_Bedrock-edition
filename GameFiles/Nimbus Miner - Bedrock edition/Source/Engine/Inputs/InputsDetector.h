#pragma once

#include "GLFW/glfw3.h"

#include <functional>
#include <vector>
#include <unordered_map>

using InputCallback = std::function<void()>;

class InputsDetector
{

public:

    static void Init(GLFWwindow* p_window);

    static GLFWwindow* GetCurrentWindow() { return _currentWindow; }
    static void SetCurrentWindow(GLFWwindow* p_window);
    
    /// <summary> Add a callback (function) on a specific input (key pressed for example) </summary>
    /// <param name="p_key"> The key ID of the key you want to link to the 'p_callback'. Use the GLFW_KEY_... macros to simplify your life. </param>
    /// <param name="p_callback"> The code that will be read if the given 'p_key' is pressed or held. </param>
    static void AddCallback(const int p_key, const InputCallback& p_callback);

    /// <summary>
    /// Detects player's inputs and launches functions bind on this key.
    /// 
    /// <para> Should be called each frame to detect inputs. </para>
    /// 
    /// <para> <b> NOTE : </b> Be sure to have set the window first,
    /// for that use <c> Init() </c> or <c> SetCurrentWindow() method. </c>
    /// </para> </summary>
    static void ProcessInputs();

private:

    static GLFWwindow* _currentWindow;
    
    static std::unordered_map<int, std::vector<InputCallback>> _keyCallbacks;
};