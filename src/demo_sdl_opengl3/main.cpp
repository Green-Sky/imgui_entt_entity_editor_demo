#include <SDL.h>
#include <glad/glad.h>

#include <imgui.h>
#include <impl/imgui_impl_opengl3.h>
#include <impl/imgui_impl_sdl.h>

#include <entt/entt.hpp>

#include <imgui_entt_entity_editor.hpp>

#include <iostream>

struct Transform {
    float x = 0.f;
    float y = 0.f;
};

struct Velocity {
    float x = 0.f;
    float y = 0.f;
};

namespace MM {
template <>
void ComponentEditorWidget<Transform>(entt::registry& reg, entt::registry::entity_type e)
{
    auto& t = reg.get<Transform>(e);
    // the "##Transform" ensures that you can use the name "x" in multiple lables
    ImGui::DragFloat("x##Transform", &t.x, 0.1f);
    ImGui::DragFloat("y##Transform", &t.y, 0.1f);
}

template <>
void ComponentEditorWidget<Velocity>(entt::registry& reg, entt::registry::entity_type e)
{
    auto& v = reg.get<Velocity>(e);
    ImGui::DragFloat("x##Velocity", &v.x, 0.1f);
    ImGui::DragFloat("y##Velocity", &v.y, 0.1f);
}
}

int main(int argc, char** argv)
{

    // setup sdl and gl
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr << "error initializing sdl\n";
        return 1;
    }

    //Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    auto* win = SDL_CreateWindow("editor_demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
    if (!win) {
        std::cerr << "error opening window\n";
        return 1;
    }

    auto* gl_context = SDL_GL_CreateContext(win);
    if (!gl_context) {
        std::cerr << "error creating gl context\n";
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "error loading gl (glad)\n";
        return 1;
    }

    if (SDL_GL_MakeCurrent(win, gl_context)) {
        std::cerr << "error making gl context current\n";
        return 1;
    }

    // setup imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplSDL2_InitForOpenGL(win, gl_context);
    ImGui_ImplOpenGL3_Init();

    // style
    {
        ImGui::StyleColorsDark();

        auto& style = ImGui::GetStyle();
        style.WindowRounding = 2.f;
    }

    entt::registry reg;

    MM::EntityEditor<entt::entity> editor;

    editor.registerComponent<Transform>("Transform");
    editor.registerComponent<Velocity>("Velocity");

    entt::entity e = entt::null;
    // main loop
    bool run = true;
    while (run) {
        // process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                run = false;
            } else {
                ImGui_ImplSDL2_ProcessEvent(&event);
            }
        }

        // imgui new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(win);
        ImGui::NewFrame();

        // ======== imgui guis go here ========

        // render editor
        editor.render(reg, e);

        // render (end frame)
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(win);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}
