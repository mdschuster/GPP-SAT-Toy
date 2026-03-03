#include <raylib.h>
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <imgui/imgui_stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include "mm/vec2.h"
#include "shape.h"
#include "rect.h"

bool applySAT(const Shape& a, const Shape& b);
bool isColliding(const Shape& a, const Shape& b);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    std::map<std::string,std::shared_ptr<Shape>> shapeMap;
    std::vector<std::string> names;

    //read configuration file
    std::ifstream myfile("assets/input.txt");
    std::string header; std::string windowName;
    //window
    int screenWidth;
    int screenHeight;
    myfile>>header>>windowName>>screenWidth>>screenHeight;
    //font
    int fontSize;
    float fontR, fontG, fontB;
    std::string fontName;
    myfile>>header>>fontName>>fontSize>>fontR>>fontG>>fontB;
    Color fontColor=ColorFromNormalized({fontR,fontG,fontB,1.0f});
    //shapes
    while(!myfile.eof()){
        //read header
        myfile>>header;
        if(strcmp(header.c_str(),"Rectangle")==0){
            float x,y,h,w,speedX,speedY, angularVelocity;
            std::string name;
            float R,G,B;
            myfile>>name>>x>>y>>speedX>>speedY>>angularVelocity>>R>>G>>B>>h>>w;
            auto* color=new float[3]{R,G,B};
            auto rect=std::make_shared<Rect>(name,x,y,h,w,speedX,speedY,color);
            rect->angularVelocity=angularVelocity;
            shapeMap[name]=rect;
            names.push_back(name);

        }
    }

    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, windowName.c_str());
    
    //initialize the raylib ImGui backend
    rlImGuiSetup(true);
    //increase ImGui item size to 2x
    ImGui::GetStyle().ScaleAllSizes(2);
    //don't write imgui.ini
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    // Set raylib to target 60 frames-per-second (this does not mean it will actually RUN at 60 fps)
    SetTargetFPS(60);               

    //load a font (Raylib gives warning if font can't be found, then uses default as fallback)
    Font font = LoadFont(fontName.c_str());

    // Statelike variables
    //--------------------------------------------------------------------------------------

    bool drawShapes=true;
    bool simulate=true;
    bool drawText=true;
    int currentItem=0;

    // Main game loop
    //--------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------


        if(simulate){
            //move Shapes
            for(const auto& n : names){
                auto s = shapeMap[n];
                if(!s->active) continue;
                s->Move(screenWidth,screenHeight);
            }
            //check collisions
            for(const auto& n : names){
                auto s = shapeMap[n];
                bool collided=false;
                if(!s->active) continue;
                for(const auto& m : names){
                    auto t = shapeMap[m];
                    if(n==m) continue;
                    if(!t->active) continue;
                    if(isColliding(*s,*t)){
                        collided=true;
                    }
                }
                if(collided){
                    s->lineColor[0]=0.6f; s->lineColor[1]=1.0f; s->lineColor[2]=0.6f;
                } else {
                    std::copy(s->color, s->color + 3, s->lineColor);
                }
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            //********** Raylib Drawing Content **********

            //draw the circle (center x, center y, radius, color)
            if(drawShapes){
                for(const auto& n : names){
                    auto s = shapeMap[n];
                    if(!s->active) continue;
                    s->Draw(font,fontSize,fontColor,drawText);
                }
            }

            //********** ImGUI Content *********

            //Draw imgui stuff last so it is over the top of everything else
            rlImGuiBegin();

                //sets the next window to be at this position
                //also uses the imgui.ini that gets created at first run
                ImGui::SetNextWindowSize(ImVec2(450, 380));
                //creates a new window
                ImGui::Begin("GPP-SAT-Toy Controls",nullptr,0);

                    //checkboxes, they directly modify the value (which is why we send a reference)
                    ImGui::SeparatorText("All Shape Controls");
                    ImGui::Checkbox("Draw Shapes",&drawShapes);
                    ImGui::SameLine();
                    ImGui::Checkbox("Draw Text",&drawText);
                    ImGui::SameLine();
                    ImGui::Checkbox("Simulate",&simulate);

                    ImGui::SeparatorText("Selected Shape Controls");

                    if (ImGui::BeginCombo("Shape", names[currentItem].c_str()))
                    {
                        for (int n = 0; n < names.size(); n++)
                        {
                            const bool is_selected = (currentItem == n);
                            if (ImGui::Selectable(names[n].c_str(), is_selected))
                                currentItem = n;

                            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    std::string currentItemName=names[currentItem];
                    auto s = shapeMap[currentItemName];
                    ImGui::Checkbox("Active",&(s->active));
                    //slider, again directly modifies the value and limits between 0 and 300 for this example
                    ImGui::SliderFloat("Scale",&(s->scaleFactor),0.1f,5.0f);

                    float v[2]={s->speedX,s->speedY};
                    ImGui::DragFloat2("Velocity",v);
                    s->speedX=v[0];
                    s->speedY=v[1];
                    ImGui::SliderFloat("Angular Velocity",&s->angularVelocity,-5.0f,5.0f);

                    //color picker button, directly modifies the color (3 element float array)
                    ImGui::ColorEdit3("Color",s->color);

                    ImGui::InputText("Name",&s->name);

                //ends this window
                ImGui::End();

            // end ImGui Content
            rlImGuiEnd();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // Clean Up
    //--------------------------------------------------------------------------------------
    rlImGuiShutdown();    // Shuts down the raylib ImGui backend
    UnloadFont(font);     // Remove font from memory
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

/**
 * Applies SAT to find separating axis
 * @param a Shape a
 * @param b Shape b
 * @return True if separating axis is found, false otherwise
 */
bool applySAT(const Shape& a, const Shape& b){
    //Loop over vertices of A
    for (size_t i = 0; i < a.vertices.size(); ++i)
    {
        //Get current edge
        Vec2 p1 = a.vertices[i];
        Vec2 p2 = a.vertices[(i + 1) % a.vertices.size()];
        Vec2 edge = p2 - p1;

        //Compute Perpendicular axis (normal)
        Vec2 normal = {-edge.y, edge.x};

        // Project vertices of A unto normal
        float minA = a.vertices[0].dot(normal);
        float maxA = minA;
        for (size_t j = 1; j < a.vertices.size(); ++j)
        {
            float p = a.vertices[j].dot(normal);
            if (p < minA) minA = p;
            if (p > maxA) maxA = p;
        }

        // Project vertices of B unto normal
        float minB = b.vertices[0].dot(normal);
        float maxB = minB;
        for (size_t j = 1; j < b.vertices.size(); ++j)
        {
            float p = b.vertices[j].dot(normal);
            if (p < minB) minB = p;
            if (p > maxB) maxB = p;
        }

        // Check for separation, if found, return true
        if (maxA < minB || maxB < minA)
            return true; // Separating axis found
    }

    // No separating axis found, return false
    return false;
}

/**
 * Computes if two shapes are colliding
 * @param a Shape a
 * @param b Shape b
 * @return True if Shapes are colliding, false otherwise
 */
bool isColliding(const Shape& a, const Shape& b){
    return !applySAT(a, b) && !applySAT(b, a);
}