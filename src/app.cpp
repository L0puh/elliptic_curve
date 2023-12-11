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

    a = -1.0f; 
    b = 1.0f; 
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


std::vector<ImVec2> App::get_points(float *start, float *end, float smooth){
    
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
        else if (y_sqr == 0 || y_sqr == -0)
            points.push_back({*end, 0});
        else if (x == -0)
            x*=-1;
        

        float y=std::sqrt(y_sqr);
        points.push_back({ x, y });
    }
    return points;
}

float App::get_a(){
    return a;
}
float App::get_b(){
    return b;
}
void App::draw_ecc(std::vector<ImVec2> points, ImVec2 pos, float *sz)
{   
    ImVec4 color = white;
    std::vector<ImVec2> points_neg;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImGuiIO& io = ImGui::GetIO();

    ImGui::BeginChild("curve", ImGui::GetWindowSize(), false, flags);
    {
        ImGui::SliderFloat("size", sz, -100.0f, 100.0f, "%.3f");

        for (size_t i = 0; i < points.size(); ++i){
            if (points.at(i).y > 0){
                points_neg.push_back({points.at(i).x, -(points.at(i).y)});
            }
        }
        for (size_t i = 1; i < points.size(); ++i)
        {
            const ImVec2& p1 = ImVec2(pos.x + points[i - 1].x * *sz, pos.y - points[i - 1].y * *sz);
            const ImVec2& p2 = ImVec2(pos.x + points[i].x * *sz, pos.y - points[i].y * *sz);
            if (points.at(i).y > 0){
                drawList->AddLine(p1, p2, ImColor(color), 3.0f ); 
            }

        }
        for (size_t i = 1; i < points_neg.size(); i++){
            /* printf("%0.3f, %0.3f\n", points_neg.at(i).x, points_neg.at(i).y); */

            const ImVec2& p1 = ImVec2(pos.x + points_neg[i - 1].x * *sz, pos.y - points_neg[i - 1].y * *sz);
            const ImVec2& p2 = ImVec2(pos.x + points_neg[i].x * *sz, pos.y - points_neg[i].y * *sz);

            drawList->AddLine(p1, p2, ImColor(color), 3.0f ); 
        }
    }
    ImGui::EndChild();
    
}


void App::main_window(){

    static float start = 2.0f;
    static float end = -1.325f;
    static float step= 0.1f;
    static float sz = 30.0f;
    
    static float num1 = 5;
    static float num2 = 10;

    ImVec2 center= ImGui::GetMainViewport()->GetCenter();

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::Begin("main", 0, flags);
    {   
        std::vector<ImVec2> points = get_points(&start, &end, step);
        ImVec2 intersection = ECC::get_intersection(points, num1, num2);
        ImGui::Text("x3: %.3f, y3: %.3f", intersection.x, intersection.y);
        ImGui::SliderFloat("num 1", &num1, 0, points.size()-1, "%.3f");
        ImGui::SliderFloat("num 2", &num2, 0, points.size()-1, "%.3f");
        draw_ecc(points, center, &sz);
        ImVec2 p1 = ImVec2(center.x + points[num1].x    * sz, center.y - points[num2].y *  sz  );
        ImVec2 p2 = ImVec2(center.x + intersection.x * sz, center.y - intersection.y *  sz);
        ImGui::GetWindowDrawList()->AddLine(p1, p2, ImColor(cyan), 3.0f);

        ImVec2 douple_p = ECC::double_point(points[num1], get_a());
        printf("DOUBLE: %0.3f, %0.3f\n", douple_p.x, douple_p.y);
        ImVec2 p3 = ImVec2(center.x + douple_p.x *sz, center.y - douple_p.y * sz);
        ImGui::GetWindowDrawList()->AddCircleFilled(p3, 3.0f, ImColor(pink), 20);
        ImGui::GetWindowDrawList()->AddLine(p1, p3, ImColor(pink), 3.0f);
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
