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

std::vector<ImVec2> get_points(float *start, float *end, float smooth){
    static float a = -1.0f; 
    static float b = 1.0f; 
    
    ImGui::SliderFloat("a", &a, -5.0f, 10.0f, "%.3f");
    ImGui::SliderFloat("b", &b, -10.0f, 10.0f, "%.3f");
    ImGui::SliderFloat("start x", start, -7.0f, 7.0f, "%.3f");
    ImGui::SliderFloat("end x", end, -7.0f, 7.0f, "%.3f");

    std::vector<ImVec2> points;
    for (float x = *start; x >= *end; x -= smooth)
    {
        float y_sqr = std::pow(x, 3) + a * x + b;
        if (y_sqr < 0) 
            y_sqr*=-1;
        float y=std::sqrt(y_sqr);
        points.push_back({ x, y });
    }
    return points;
}

void App::draw_ecc(std::vector<ImVec2> points, ImVec2 pos)
{   
    static float sz = 30.0f;
    ImVec4 color = white;
    std::vector<ImVec2> points_neg;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::BeginChild("curve", ImGui::GetWindowSize(), false, flags);
    {
        ImGui::SliderFloat("size", &sz, -100.0f, 100.0f, "%.3f");

        for (size_t i = 1; i < points.size(); ++i){
            if (points.at(i).y > 0){
                points_neg.push_back({points.at(i).x, -(points.at(i).y)});
            }
        }
        for (size_t i = 1; i < points.size(); ++i)
        {
            const ImVec2& p1 = ImVec2(pos.x + points[i - 1].x * sz, pos.y - points[i - 1].y * sz);
            const ImVec2& p2 = ImVec2(pos.x + points[i].x * sz, pos.y - points[i].y * sz);
            if (points.at(i).y > 0){
                drawList->AddLine(p1, p2, ImColor(color), 3.0f ); 
                ImGui::Text("p2: %0.4f, %0.4f", p2.x, p2.y);
                ImGui::Text("p1: %0.4f, %0.4f", p1.x, p1.y);
                }
            }
        for (size_t i =1; i < points_neg.size(); i++){
            printf("%0.3f, %0.3f\n", points_neg.at(i).x, points_neg.at(i).y);

            const ImVec2& p1 = ImVec2(pos.x + points_neg[i - 1].x * sz, pos.y - points_neg[i - 1].y * sz);
            const ImVec2& p2 = ImVec2(pos.x + points_neg[i].x * sz, pos.y - points_neg[i].y * sz);

            drawList->AddLine(p1, p2, ImColor(color), 3.0f ); 
        }
    }
    ImGui::EndChild();
    
}


void App::main_window(){

    static float start = 2.0f;
    static float end = -1.325f;
    static float step= 0.1f;
    ImVec2 center= ImGui::GetMainViewport()->GetCenter();

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::Begin("main", 0, flags);
    {
        draw_ecc(get_points(&start, &end, step), center);
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
