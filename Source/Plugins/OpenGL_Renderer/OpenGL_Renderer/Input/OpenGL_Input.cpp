#include "OpenGL_Input.h"

#include "OpenGL/GLFW/glfw3.h"
#include "Core/MainSingleton.h"

#include "Renderer/RendererManager.h"
#include "OpenGL_Renderer/OpenGL_Renderer.h"

#include "ImGui/imgui.h"

namespace Eclipse::OpenGL::Input
{
#ifdef ECL_EDITOR
    ImGuiKey GlfwToImGuiKey(int key)
    {
        switch (key)
        {
        case GLFW_KEY_TAB:        return ImGuiKey_Tab;
        case GLFW_KEY_LEFT:       return ImGuiKey_LeftArrow;
        case GLFW_KEY_RIGHT:      return ImGuiKey_RightArrow;
        case GLFW_KEY_UP:         return ImGuiKey_UpArrow;
        case GLFW_KEY_DOWN:       return ImGuiKey_DownArrow;

        case GLFW_KEY_PAGE_UP:    return ImGuiKey_PageUp;
        case GLFW_KEY_PAGE_DOWN:  return ImGuiKey_PageDown;
        case GLFW_KEY_HOME:       return ImGuiKey_Home;
        case GLFW_KEY_END:        return ImGuiKey_End;
        case GLFW_KEY_INSERT:     return ImGuiKey_Insert;
        case GLFW_KEY_DELETE:     return ImGuiKey_Delete;
        case GLFW_KEY_BACKSPACE:  return ImGuiKey_Backspace;
        case GLFW_KEY_SPACE:      return ImGuiKey_Space;
        case GLFW_KEY_ENTER:      return ImGuiKey_Enter;
        case GLFW_KEY_ESCAPE:     return ImGuiKey_Escape;

        case GLFW_KEY_A: return ImGuiKey_A;
        case GLFW_KEY_B: return ImGuiKey_B;
        case GLFW_KEY_C: return ImGuiKey_C;
        case GLFW_KEY_D: return ImGuiKey_D;
        case GLFW_KEY_E: return ImGuiKey_E;
        case GLFW_KEY_F: return ImGuiKey_F;
        case GLFW_KEY_G: return ImGuiKey_G;
        case GLFW_KEY_H: return ImGuiKey_H;
        case GLFW_KEY_I: return ImGuiKey_I;
        case GLFW_KEY_J: return ImGuiKey_J;
        case GLFW_KEY_K: return ImGuiKey_K;
        case GLFW_KEY_L: return ImGuiKey_L;
        case GLFW_KEY_M: return ImGuiKey_M;
        case GLFW_KEY_N: return ImGuiKey_N;
        case GLFW_KEY_O: return ImGuiKey_O;
        case GLFW_KEY_P: return ImGuiKey_P;
        case GLFW_KEY_Q: return ImGuiKey_Q;
        case GLFW_KEY_R: return ImGuiKey_R;
        case GLFW_KEY_S: return ImGuiKey_S;
        case GLFW_KEY_T: return ImGuiKey_T;
        case GLFW_KEY_U: return ImGuiKey_U;
        case GLFW_KEY_V: return ImGuiKey_V;
        case GLFW_KEY_W: return ImGuiKey_W;
        case GLFW_KEY_X: return ImGuiKey_X;
        case GLFW_KEY_Y: return ImGuiKey_Y;
        case GLFW_KEY_Z: return ImGuiKey_Z;

        case GLFW_KEY_0: return ImGuiKey_0;
        case GLFW_KEY_1: return ImGuiKey_1;
        case GLFW_KEY_2: return ImGuiKey_2;
        case GLFW_KEY_3: return ImGuiKey_3;
        case GLFW_KEY_4: return ImGuiKey_4;
        case GLFW_KEY_5: return ImGuiKey_5;
        case GLFW_KEY_6: return ImGuiKey_6;
        case GLFW_KEY_7: return ImGuiKey_7;
        case GLFW_KEY_8: return ImGuiKey_8;
        case GLFW_KEY_9: return ImGuiKey_9;

        case GLFW_KEY_LEFT_SHIFT:  return ImGuiKey_LeftShift;
        case GLFW_KEY_LEFT_CONTROL:return ImGuiKey_LeftCtrl;
        case GLFW_KEY_LEFT_ALT:    return ImGuiKey_LeftAlt;
        case GLFW_KEY_LEFT_SUPER:  return ImGuiKey_LeftSuper;

        case GLFW_KEY_RIGHT_SHIFT:  return ImGuiKey_RightShift;
        case GLFW_KEY_RIGHT_CONTROL:return ImGuiKey_RightCtrl;
        case GLFW_KEY_RIGHT_ALT:    return ImGuiKey_RightAlt;
        case GLFW_KEY_RIGHT_SUPER:  return ImGuiKey_RightSuper;

        case GLFW_KEY_F1:  return ImGuiKey_F1;
        case GLFW_KEY_F2:  return ImGuiKey_F2;
        case GLFW_KEY_F3:  return ImGuiKey_F3;
        case GLFW_KEY_F4:  return ImGuiKey_F4;
        case GLFW_KEY_F5:  return ImGuiKey_F5;
        case GLFW_KEY_F6:  return ImGuiKey_F6;
        case GLFW_KEY_F7:  return ImGuiKey_F7;
        case GLFW_KEY_F8:  return ImGuiKey_F8;
        case GLFW_KEY_F9:  return ImGuiKey_F9;
        case GLFW_KEY_F10: return ImGuiKey_F10;
        case GLFW_KEY_F11: return ImGuiKey_F11;
        case GLFW_KEY_F12: return ImGuiKey_F12;
        }

        return ImGuiKey_None;
    }
#endif

	OpenGL_Input* GetInputPtrFromGlfwPtr(GLFWwindow* w) {
		return (OpenGL_Input*)glfwGetWindowUserPointer(w);
	}

    void OpenGL_Input::Init()
    {
        Graphics::OpenGL::OpenGL_Renderer& r =
            (Graphics::OpenGL::OpenGL_Renderer&)Eclipse::Graphics::RendererManager::GetRenderer();

        GLFWwindow* window = r.GetWindow();

        glfwSetWindowUserPointer(window, this);

        glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods)
            {
                OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);

                if (input)
                    input->OnKey_Callback(w, key, scancode, action, mods);

#ifdef ECL_EDITOR
                ImGuiIO& io = ImGui::GetIO();

                ImGuiKey imguiKey = GlfwToImGuiKey(key);

                if (imguiKey != ImGuiKey_None)
                {
                    io.AddKeyEvent(
                        imguiKey,
                        action != GLFW_RELEASE
                    );
                }
#endif
            });

        glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y)
            {
                OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);

                if (input)
                    input->OnMousePos_Callback(w, x, y);

                // Removed because moving camera in scene view, this makes camera jump a bunch of units in the oposite direction (Do not know why)
//#ifdef ECL_EDITOR
//                ImGui::GetIO().AddMousePosEvent(
//                    (float)x,
//                    (float)y
//                );
//#endif
            });

        glfwSetCursorEnterCallback(window, [](GLFWwindow* w, int entered)
            {
                OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);

                if (input)
                    input->OnMouseEnter_Callback(w, entered);

#ifdef ECL_EDITOR
                ImGui::GetIO().AddMouseSourceEvent(
                    ImGuiMouseSource_Mouse
                );

                if (!entered)
                {
                    ImGui::GetIO().AddMousePosEvent(-FLT_MAX, -FLT_MAX);
                }
#endif
            });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods)
            {
                OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);

                if (input)
                    input->OnMouseButton_Callback(w, button, action, mods);

#ifdef ECL_EDITOR
                if (button >= GLFW_MOUSE_BUTTON_1 &&
                    button <= GLFW_MOUSE_BUTTON_8)
                {
                    ImGui::GetIO().AddMouseButtonEvent(
                        button,
                        action == GLFW_PRESS
                    );
                }
#endif
            });

        glfwSetWindowFocusCallback(window, [](GLFWwindow* w, int focused)
            {
                OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);

                if (input)
                    input->OnWindowFocus_Callback(w, focused);

#ifdef ECL_EDITOR
                ImGui::GetIO().AddFocusEvent(focused);
#endif
            });

        glfwSetScrollCallback(window, [](GLFWwindow* w, double xOffset, double yOffset)
            {
                OpenGL_Input* input = GetInputPtrFromGlfwPtr(w);

                if (input)
                    input->OnMouseScroll_Callback(w, xOffset, yOffset);

#ifdef ECL_EDITOR
                ImGui::GetIO().AddMouseWheelEvent(
                    (float)xOffset,
                    (float)yOffset
                );
#endif
            });
    }
}