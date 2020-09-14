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


#include <velocity.hpp>


void main_loop(void* arg);

// globals
SDL_Window* g_win = nullptr;
SDL_GLContext g_gl_context;

entt::registry reg;
MM::EntityEditor<entt::entity> editor;

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


	editor.registerComponent<Components::Transform>("Transform");
	editor.registerComponent<Components::Velocity>("Velocity");

	for (size_t i = 0; i < 500; i++) {
		e = reg.create();
		// setup nice initial entity
		{
			//reg.emplace<Components::Transform>(e, 500.f, 500.f);
			//reg.emplace<Components::Velocity>(e, 500.f, 500.f);
			reg.emplace<Components::Transform>(e, float(rand()%5000)/10, float(rand()%5000)/10);
			reg.emplace<Components::Velocity>(e, float((rand()%5000) - 2500)/10, float((rand()%5000) - 2500)/10);
		}
	}


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

	Systems::Velocity(reg, 1.f/60.f);

	// imgui new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(g_win);
	ImGui::NewFrame();

	// ======== imgui guis go here ========

	// debug draw entities
	{
		auto* dl = ImGui::GetBackgroundDrawList();
		reg.view<Components::Transform>().each(
		[&](auto e, Components::Transform& trans) {
			auto e_int = entt::to_integral(e) + 1;

			// generate color based on id
			auto col = IM_COL32((13*e_int)%256,(159*e_int)%256,(207*e_int)%256,250);

			dl->AddCircleFilled(
				ImVec2(trans.x, trans.y),
				10.f,
				col
			);
		});
	}

	// render editor
	editor.renderSimpleCombo(reg, e);

	// render (end frame)
	ImGui::Render();
	SDL_GL_MakeCurrent(g_win, g_gl_context);
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(g_win);
}

