#ifndef ELLEPTIC_CURVE
#define ELLEPTIC_CURVE

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <fstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "imgui_internal.h"

class App {
    private:
        ImGuiWindowFlags flags = 0;
        ImGuiInputTextFlags text_flags = 0; 

        // colors 
        ImVec4 white = ImVec4(255.0f, 255.0f, 255.0f, 1.0f);
        ImVec4 black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        ImVec4 pink = ImVec4(60.0f, 0.0f, 173.0f, 1.0f);

        GLFWwindow *window;
        bool open_new_window;
    public:
        App(int width, int height);
        ~App();

    private:
        void save_file(const char* namefile);
        void menu();
        void main_window();
        void draw_ecc(ImDrawList* drawList, float startX, float endX, float stepSize, ImVec2 center);
    public:
        void init_app();
        void run_app();
};

class ECC {
    public:
        static int get_x();
        static int get_y();
};

#endif
