#include <SDL.h>
#include <glad/glad.h>

#include <imgui.h>
#include <impl/imgui_impl_opengl3.h>
#include <impl/imgui_impl_sdl.h>

#include <entt/entt.hpp>

#include <imgui_entt_entity_editor.hpp>

#include <iostream>

#include <velocity.hpp>


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

	editor.registerComponent<Components::Transform>("Transform");
	editor.registerComponent<Components::Velocity>("Velocity");

	entt::entity e = reg.create();
	// setup nice initial entity
	{
		reg.emplace<Components::Transform>(e, 500.f, 500.f);
		reg.emplace<Components::Velocity>(e, 500.f, 500.f);
	}

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

		Systems::Velocity(reg, 1.f/60.f);

		// imgui new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(win);
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
