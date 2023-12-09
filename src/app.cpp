#include "elliptic_curve.h"
#include "imgui.h"
#include <cmath>
#include <cstdio>
#include <vector>

App::App(int width, int height){
    if (!glfwInit())
        exit(1);
    window = glfwCreateWindow(width, height, 
            "elliptic curve", nullptr, nullptr);

    flags = 0;
    flags |= ImGuiWindowFlags_AlwaysAutoResize;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoDecoration;
    text_flags |= ImGuiInputTextFlags_AllowTabInput;
    open_new_window = false;
}

App::~App(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::init_app(){
    glfwMakeContextCurrent(window);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void App::run_app(){
    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        menu(); 
        main_window();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

void App::draw_ecc(ImDrawList* drawList, float startX, float endX, float stepSize, ImVec2 center)
{   
    ImGui::BeginChild("curve", ImGui::GetWindowSize(), true, flags);
    {
        static float sz = 20.0f;
        static float a = -1.0f; 
        static float b = 1.0f; 
        ImGuiIO& ii = ImGui::GetIO();
        std::vector<ImVec2> points;
        std::vector<ImVec2> points_neg;

        ImGui::SliderFloat("size", &sz, -100.0f, 100.0f, "%.3f");
        ImGui::SliderFloat("a", &a, -5.0f, 5.0f, "%.3f");
        ImGui::SliderFloat("b", &b, -5.0f, 5.0f, "%.3f");
        ImGui::SliderFloat("start x", &startX, -5.0f, 5.0f, "%.3f");
        ImGui::SliderFloat("end x", &endX, -5.0f, 5.0f, "%.3f");
        for (float x = startX; x >= endX; x -= stepSize)
        {
            float y_sqr = std::pow(x, 3) + a * x + b;
            if (y_sqr < 0) 
                y_sqr*=-1;
            float y=std::sqrt(y_sqr);
            points.push_back({ x, y });
        }
        // mirror of curve
        for (size_t i = 1; i < points.size(); ++i){
            if (points.at(i).y > 0){
                points_neg.push_back({points.at(i).x, -(points.at(i).y)});
            }
        }
        ImVec4 color = white;
        for (size_t i = 1; i < points.size(); ++i)
        {
            const ImVec2& p1 = ImVec2(center.x + points[i - 1].x * sz, center.y - points[i - 1].y * sz);
            const ImVec2& p2 = ImVec2(center.x + points[i].x * sz, center.y - points[i].y * sz);
            if (points.at(i).y > 0){
                drawList->AddLine(p1, p2, ImColor(color), 3.0f ); 
                ImGui::Text("p2: %0.4f, %0.4f", p2.x, p2.y);
                ImGui::Text("p1: %0.4f, %0.4f", p1.x, p1.y);
                }
            }
        for (size_t i =1; i < points_neg.size(); i++){
            printf("%0.3f, %0.3f\n", points_neg.at(i).x, points_neg.at(i).y);
            const ImVec2& p1 = ImVec2(center.x + points_neg[i - 1].x * sz, center.y - points_neg[i - 1].y * sz);
            const ImVec2& p2 = ImVec2(center.x + points_neg[i].x * sz, center.y - points_neg[i].y * sz);
           drawList->AddLine(p1, p2, ImColor(color), 3.0f ); 
        }
    }
    ImGui::EndChild();
}


void App::main_window(){
    ImGuiIO& io = ImGui::GetIO();
    float startX = 2.0f;
    float endX = -1.325f;
    float stepSize = 0.1f;
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    {
        ImGui::Begin("main", 0, flags);
        ImDrawList* draw_list= ImGui::GetWindowDrawList();
        ImGui::Text("curve");
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImVec2 s = ImGui::GetMainViewport()->Pos;
        float x = center.x;
        float y = p.y;
        draw_ecc(draw_list, startX, endX, stepSize, center);

        ImGui::End();
    }
}



void App::menu(){
    ImGui::Begin("menu", 0, flags);
    if(ImGui::BeginMainMenuBar()){
        if(ImGui::BeginMenu("menu")){
            if(ImGui::Button("config")) {
                text_flags = ImGuiInputTextFlags_AllowTabInput;
                open_new_window=true;
            }
            if(ImGui::MenuItem("close")){ exit(0); }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        if(open_new_window)
            save_file("file.txt");

        ImGui::End();
    }
}
void App::save_file(const char* filename){
    ImGui::Begin("file", &open_new_window, flags);
    {
        ImGui::OpenPopup("editor");
        if(ImGui::BeginPopup("editor", ImGuiWindowFlags_AlwaysAutoResize)){
            static char buff[400] ={};
            ImGui::InputTextMultiline("text", buff, IM_ARRAYSIZE(buff), 
                    ImVec2(300.0f, 300.0f), 
                    text_flags);
            if (ImGui::Button("save")){
                text_flags|=ImGuiInputTextFlags_ReadOnly;
                if (*buff){
                    std::ofstream file(filename);
                    if (file.is_open()){
                        file << buff << "\n";
                        file.close();
                        }
                    }
                ImGui::CloseCurrentPopup();
                open_new_window=false;
                }
        ImGui::EndPopup();
        }
    ImGui::End();
    }
}
