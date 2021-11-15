//
// Created by Lucki on 14-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_GUI_H
#define ITU_GRAPHICS_PROGRAMMING_GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entities/light.h>
#include "../config.h"

class Gui {
    private:
        Config* config;
        Light* sun;
    public:
        Gui(Config* config, Light* sun) {
            this->config = config;
            this->sun = sun;
        }

        void drawGui() {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Terrain settings");
                ImGui::Text("Generation");
                if(ImGui::RadioButton("NORMAL", config->generationSetting == NORMAL)) {
                    config->generationSetting = NORMAL;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                } ImGui::SameLine();
                if(ImGui::RadioButton("VERTEX_COPY", config->generationSetting == VERTEX_COPY)) {
                    config->generationSetting = VERTEX_COPY;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                } ImGui::SameLine();
                if(ImGui::RadioButton("GEOMETRY", config->generationSetting == GEOMETRY)) {
                    config->generationSetting = GEOMETRY;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                } ImGui::SameLine();
                if(ImGui::RadioButton("MESH", config->generationSetting == MESH))  {
                    config->generationSetting = MESH;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } ImGui::SameLine();
                ImGui::Separator();
                ImGui::Text("Lighting");
                ImGui::ColorEdit3("light color", (float*)&sun->colour);
                ImGui::SliderFloat("light intensity", (float*)&sun->intensity, 0.0f, 1.0f);
                ImGui::DragFloat3("light position", (float*)&sun->position, config->size / 100.0f, -config->size, config->size);
                ImGui::SliderFloat("ambient reflectance", &config->ambientReflectance, 0.0f, 1.0f);
                ImGui::SliderFloat("diffuse reflectance", &config->diffuseReflectance, 0.0f, 1.0f);
                ImGui::Separator();
                ImGui::SliderFloat("Frequency", &config->frequency, 0.01f, 1.0f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void clean() {
            // Cleanup
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }
};

#endif //ITU_GRAPHICS_PROGRAMMING_GUI_H
