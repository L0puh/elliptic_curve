#ifndef ELLEPTIC_CURVE
#define ELLEPTIC_CURVE

#include <GLFW/glfw3.h>

#include <cstdlib>
#include <fstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>
#include "imgui_internal.h"

class App {
    private:
        float a; 
        float b; 
        ImGuiWindowFlags flags = 0;
        ImGuiInputTextFlags text_flags = 0; 

        // colors 
        ImVec4 white = ImVec4(255.0f, 255.0f, 255.0f, 1.0f);
        ImVec4 black = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        ImVec4 pink = ImVec4(60.0f, 0.0f, 173.0f, 1.0f);
        ImVec4 cyan = ImVec4(0.0f, 255.0f, 128.0f, 1.0f);

        GLFWwindow *window;
        bool open_new_window;
    public:
        App(int width, int height);
        ~App();

    private:
        void save_file(const char* namefile);
        void menu();
        void main_window();
        void draw_ecc(std::vector<ImVec2> points, ImVec2 pos, float *sz);
        std::vector<ImVec2> get_points(float *start, float *end, float smooth);
    public:
        void init_app();
        void run_app();
    public:
        float get_a();
        float get_b();
};

class ECC {
    public:
        static ImVec2 get_intersection(std::vector<ImVec2> points, int num, int num2);
        static ImVec2 double_point(ImVec2 point, float a);
};


#endif
