#include "Engine/Engine.h"

int main(int argc, char** argv) {
	Engine::Init();
	Engine::MainRow = Engine::ScriptLoadLua("Engine.lua");

	std::string window_name;
	int window_width, window_height;
	
	if (Engine::ScriptCheckLua()) {
		lua_getglobal(Engine::LuaState, "Engine");
		if (lua_istable(Engine::LuaState, -1)) {
			lua_pushstring(Engine::LuaState, "WindowName");
			lua_gettable(Engine::LuaState, -2);
			window_name = lua_tostring(Engine::LuaState, -1);
			lua_pop(Engine::LuaState, 1);

			lua_pushstring(Engine::LuaState, "WindowWidth");
			lua_gettable(Engine::LuaState, -2);
			window_width = lua_tointeger(Engine::LuaState, -1);
			lua_pop(Engine::LuaState, 1);

			lua_pushstring(Engine::LuaState, "WindowHeight");
			lua_gettable(Engine::LuaState, -2);
			window_height = lua_tointeger(Engine::LuaState, -1);
			lua_pop(Engine::LuaState, 1);
		}
	}

	SDL_Window* window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	Engine::Scene default_scene;

	bool running = true;
    while(running){
        // Events
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            switch(event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    default_scene.InputDown(event.key.keysym.sym);
                    break;

                case SDL_KEYUP:
                    default_scene.InputUp(event.key.keysym.sym);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    default_scene.MouseDown(event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    default_scene.MouseUp(event.button);
                    break;
                default:
                    break;
            }
        }

        /* Update */
        default_scene.Update();

        default_scene.Render();

        // Debbuug
        SDL_RenderPresent(renderer);
        SDL_Delay(16.7);
    }

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	Engine::End();

	return 0;
}