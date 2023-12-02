#ifndef ELLEPTIC_CURVE
#define ELLEPTIC_CURVE

#include <cstdlib>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class App {
    private:
        ImGuiWindowFlags flags = 0;
        ImGuiInputTextFlags text_flags = 0; 
        ImVec4 clear_colors;
        GLFWwindow *window;
        bool open_new_window;
    public:
        App();
        ~App();

    public:
        void init_app();
        void run_app();
        void menu();
};

#endif
