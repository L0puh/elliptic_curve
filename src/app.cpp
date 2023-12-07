#include "elliptic_curve.h"
#include "imgui.h"
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

//FIXME
    float a = 3.0f; 
    float b = 3.0f; 
    float p = 407.0f; 

    std::vector<ImVec2> points;
    for (float x = startX; x <= endX; x += stepSize)
    {
        float y_squared = std::fmod(std::pow(x, 3) + a * x + b, p);
        float y = std::sqrt(y_squared);
        points.push_back({ x, y });
    }
    for (size_t i = 1; i < points.size(); ++i)
    {
        const ImVec2& p1 = ImVec2(center.x + points[i - 1].x, center.y - points[i - 1].y);
        const ImVec2& p2 = ImVec2(center.x + points[i].x, center.y - points[i].y);
        drawList->AddLine(p1, p2, ImColor(pink) ); 
    }
}

void App::main_window(){
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(550, 550), ImGuiCond_Always);
    {
    ImGui::Begin("main", 0, flags);
    ImDrawList* draw_list= ImGui::GetWindowDrawList();
    ImGui::Text("curve");
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 s = ImGui::GetMainViewport()->Pos;
    float x = center.x;
    float y = p.y;
    draw_list->AddLine(ImVec2(x, y), ImVec2(x, y * 6.0f), ImColor(white), 2.0f); // vertical
    draw_list->AddLine(ImVec2(p.x, center.y), ImVec2(x * 6.0f, center.y), ImColor(white), 2.0f); // horizontal
    int func[5];
    for (int i = 0; i < 5; i++){
       func[i] = std::pow(y, 2) - std::pow(x, 3) - x *(-1) - 1;
    }
    float startX = -40.0f;
    float endX = 40.0f;
    float stepSize = 40.0f;

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
