#include "elliptic_curve.h"

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

void App::main_window(){
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.55f, io.DisplaySize.y * 0.45f), 
            ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowWidth(), 
                                    ImGui::GetWindowHeight()), 
                                    ImGuiCond_None);
    {
    ImGui::Begin("main", 0, flags);
    ImDrawList* draw_list= ImGui::GetWindowDrawList();
    ImGui::Text("curve");
    ImVec2 p = ImGui::GetCursorScreenPos();
    draw_list->AddBezierCubic(io.MouseClickedPos[0],
                              ImVec2(p.x+3, p.y+3),
                              ImVec2(p.x+4, p.y+8), 
                              io.MouseClickedPos[1], 
                              ImColor(white), 2.0f, 32);
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
