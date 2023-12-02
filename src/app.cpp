#include "elliptic_curve.h"
#include "imgui.h"
#include <fstream>

App::App(){
    if (!glfwInit())
        exit(1);
    window = glfwCreateWindow(600, 600, "elliptic curve", nullptr, nullptr);
    clear_colors = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       
        glfwSwapBuffers(window);
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
            if(ImGui::MenuItem("close")){ 
                 exit(0); 
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
        ImGui::End();
    }
    if (open_new_window){

            ImGui::Begin("file", &open_new_window, flags);
            {
                ImGui::OpenPopup("editor");
                if(ImGui::BeginPopup("editor", ImGuiWindowFlags_AlwaysAutoResize)){
                    static char buff[100] ={};
                    ImGui::InputTextMultiline("text", buff, IM_ARRAYSIZE(buff), 
                            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight()*16), 
                            text_flags);
                    if (ImGui::Button("save")){
                        text_flags|=ImGuiInputTextFlags_ReadOnly;
                        if (*buff){
                            std::ofstream file("file.txt");
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
    }
