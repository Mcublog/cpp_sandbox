/**
 * @file mydemo.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-11-15
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include <cmath>
#include <string>

#include "imgui.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME demo
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------
void OpenFileDialog()
{
    system("zenity --file-selection --title='Выберите файл' > /tmp/tset_file_path.txt");

    FILE *file;
    const char *filename = "/tmp/tset_file_path.txt";
    char buffer[256];

    // Открываем файл в режиме чтения ("r")
    file = fopen(filename, "r");
    if (file == NULL) {
        LOG_ERROR("Ошибка открытия файла");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        LOG_INFO("%s", buffer);
    }

    fclose(file);
}

void ImGui::ShowDemoWindow(bool *p_open)
{
    float my_color[4] = {};
    // Create a window called "My First Tool", with a menu bar.
    ImGui::Begin("My First Tool", p_open,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoMove);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O"))
            { /* Do stuff */
                OpenFileDialog();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            { /* Do stuff */
            }
            if (ImGui::MenuItem("Close", "Ctrl+W"))
            {
                *p_open = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Edit a color stored as 4 floats
    ImGui::ColorEdit4("Color", my_color);

    // Generate samples and plot them
    float samples[100];
    for (int n = 0; n < 100; n++)
        samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++)
        ImGui::Text("%04d: Какой-то текст", n);
    ImGui::EndChild();
    ImGui::End();
}
