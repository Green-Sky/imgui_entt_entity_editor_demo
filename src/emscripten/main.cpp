#include <imgui.h>
#include <impl/imgui_impl_sdl.h>
#include <impl/imgui_impl_opengl3.h>

//#include <stdio.h>

#include <emscripten.h>


#include <SDL.h>
#include <SDL_opengles2.h>
//#include <glad/glad.h>


#include <entt/entt.hpp>

#include <iostream>

#include <imgui_entt_entity_editor.hpp>




struct Transform {
	float x = 0.f;
	float y = 0.f;
};

struct Velocity {
	float x = 0.f;
	float y = 0.f;
};

namespace Widgets {
	void Velocity(::Velocity& v) {
		ImGui::DragFloat("x##Velocity", &v.x, 0.1f);
		ImGui::DragFloat("y##Velocity", &v.y, 0.1f);
	}
} // Widgets

void main_loop(void* arg);

// globals
SDL_Window* g_win = nullptr;
SDL_GLContext g_gl_context;

entt::registry reg;
MM::ImGuiEntityEditor<decltype(reg)> editor;

entt::entity e = entt::null;

int main(int argc, char** argv) {

	// setup sdl and gl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER)) {
		std::cerr << "error initializing sdl\n";
		return 1;
	}

	//const char* glsl_version = "#version 300 es";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	g_win = SDL_CreateWindow("editor_demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL);
	if (!g_win) {
		std::cerr << "error opening window\n";
		return 1;
	}

	g_gl_context = SDL_GL_CreateContext(g_win);
	if (!g_gl_context) {
		std::cerr << "error creating webgl context\n";
		return 1;
	}

	//if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
		//std::cerr << "error loading gl (glad)\n";
		//return 1;
	//}

	if (SDL_GL_MakeCurrent(g_win, g_gl_context)) {
		std::cerr << "error making gl context current\n";
		return 1;
	}

	// setup imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplSDL2_InitForOpenGL(g_win, g_gl_context);
	ImGui_ImplOpenGL3_Init();

	// style
	{
		ImGui::StyleColorsDark();

		auto& style = ImGui::GetStyle();
		style.WindowRounding = 2.f;
	}


	// "registerTrivial" registers the type, name, create and destroy functions for trivialy costructable(and destroyable) types.
	// you just need to provide a "widget" function if you use this method.
	editor.registerTrivial<Transform>(reg, "Transform");
	editor.registerTrivial<Velocity>(reg, "Velocity");

	editor.registerComponentWidgetFn(
		reg.type<Transform>(),
		[](entt::registry& reg, auto e) {
			auto& t = reg.get<Transform>(e);

			// the "##Transform" ensures that you can use the name "x" in multiple lables
			ImGui::DragFloat("x##Transform", &t.x, 0.1f);
			ImGui::DragFloat("y##Transform", &t.y, 0.1f);
		});

	editor.registerComponentWidgetFn(
		reg.type<Velocity>(),
		[](entt::registry& reg, auto e) {
			auto& v = reg.get<Velocity>(e);
			Widgets::Velocity(v);
		});


	// main loop
	//bool run = true;
	//while (run) {
		//// process events
		//SDL_Event event;
		//while (SDL_PollEvent(&event)) {
			//if (event.type == SDL_QUIT) {
				//run = false;
			//} else {
				//ImGui_ImplSDL2_ProcessEvent(&event);
			//}
		//}

		//// imgui new frame
		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplSDL2_NewFrame(win);
		//ImGui::NewFrame();

		//// ======== imgui guis go here ========

		//// render editor
		//editor.renderImGui(reg, e);

		//// render (end frame)
		//ImGui::Render();
		//glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//SDL_GL_SwapWindow(win);
	//}


	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplSDL2_Shutdown();
	//ImGui::DestroyContext();

	//SDL_GL_DeleteContext(gl_context);
	//SDL_DestroyWindow(win);
	//SDL_Quit();

	//return 0;
	emscripten_set_main_loop_arg(main_loop, NULL, 0, true);
}

void main_loop(void* arg) {
	ImGuiIO& io = ImGui::GetIO();
	IM_UNUSED(arg);

	// process events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);
	}

	// imgui new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(g_win);
	ImGui::NewFrame();

	// ======== imgui guis go here ========

	// render editor
	editor.renderImGui(reg, e);

	// render (end frame)
	ImGui::Render();
	SDL_GL_MakeCurrent(g_win, g_gl_context);
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(g_win);
}

