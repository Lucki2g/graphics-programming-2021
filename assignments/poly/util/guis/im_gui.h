//
// Created by Lucki on 14-11-2021.
//

#ifndef ITU_GRAPHICS_PROGRAMMING_IM_GUI_H
#define ITU_GRAPHICS_PROGRAMMING_IM_GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entities/light.h>
#include <entities/entity.h>
#include <engine/master_renderer.h>
#include "config.h"

class Gui {
    private:
        Config* config;
        Light* sun;
        MasterRenderer* renderer;

    public:
        Gui(Config* config, Light* sun, MasterRenderer* renderer) {
            this->config = config;
            this->sun = sun;
            this->renderer = renderer;
        }

        void drawGui(Loader* loader, ColourGenerator* colourGenerator, MeshGenerator* meshGenerator, Config* config) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Terrain settings");
                ImGui::Text("Terrain Mode");
                if(ImGui::RadioButton("NORMAL", config->generationSetting == NORMAL)) {
                    config->generationSetting = NORMAL;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if(ImGui::RadioButton("VERTEX_COPY", config->generationSetting == VERTEX_COPY)) {
                    config->generationSetting = VERTEX_COPY;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if(ImGui::RadioButton("GEOMETRY", config->generationSetting == GEOMETRY)) {
                    config->generationSetting = GEOMETRY;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if(ImGui::RadioButton("FLAT", config->generationSetting == FLAT))  {
                    config->generationSetting = FLAT;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if(ImGui::RadioButton("SPECIAL_FLAT", config->generationSetting == FLAT_S))  {
                    config->generationSetting = FLAT_S;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if(ImGui::RadioButton("EQUILINOX", config->generationSetting == EQUILINOX))  {
                    config->generationSetting = EQUILINOX;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                if(ImGui::RadioButton("MESH", config->generationSetting == MESH))  {
                    config->generationSetting = MESH;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                ImGui::Separator();
                ImGui::Text("Lighting");
                ImGui::ColorEdit3("light color", (float*)&sun->colour);
                ImGui::SliderFloat("light intensity", (float*)&sun->intensity, 0.0f, 1.0f);
                ImGui::DragFloat3("light direction", (float*)&config->lightDirection, 0.01f, -1.0f, 1.0f);
                ImGui::SliderFloat("ambient reflectance", &config->ambientReflectance, 0.0f, 1.0f);
                ImGui::SliderFloat("diffuse reflectance", &config->diffuseReflectance, 0.0f, 1.0f);
                ImGui::SliderFloat("specular reflectance", &config->specularReflectance, 0.0f, 1.0f);
                ImGui::SliderFloat("specular factor", &config->specularFactor, 2.0f, 256.0f);
                ImGui::Separator();
                ImGui::Text("Water");
                ImGui::DragFloat3("water color", (float*)&config->waterColour, 0.01f, 0.0f, 1.0f);
                ImGui::SliderFloat("fresnel", &config->fresnelReflectiveness, 0.0f, 1.0f);
                ImGui::Checkbox("Show/Hide FBOs", &config->showFbos);

                ImGui::Text("Distortion");
                if(ImGui::RadioButton("RANDOM", config->distortionFunction == RANDOM)) {
                    config->distortionFunction = RANDOM;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }
                ImGui::SameLine();
                if(ImGui::RadioButton("SINE WAVE", config->distortionFunction == SINUS)) {
                    config->distortionFunction = SINUS;
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }

                ImGui::DragFloat("wave length", (float*)&config->waveLength, 1.0f, 1.0f, 10.0f);
                ImGui::DragFloat("wave amplitude", (float*)&config->waveAmplitude, 0.1f, 0.1f, 1.0f);
                //ImGui::DragFloat("near plane", (float*)&config->near_plane, 0.005f, 0.01f, 0.5f);
                //ImGui::DragFloat("far plane", (float*)&config->far_plane, 1.0f, 1.0f, 1000.0f);
                /**ImGui::Separator();
                ImGui::Text("Window");
                ImGui::Checkbox("unlock FPS", &config->unlockFPS);*/
                //ImGui::DragFloat("size", (float*)&config->terrain_size, 1.0f, 40.0f, 400.0f);
                //if (ImGui::Button("Regenerate terrain")) { renderer->regenerate(loader, colourGenerator, meshGenerator, config); }

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

#endif //ITU_GRAPHICS_PROGRAMMING_IM_GUI_H
