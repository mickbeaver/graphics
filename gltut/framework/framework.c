#include <assert.h>
#include <stdbool.h>
#include "SDL.h"
#include "framework.h"

static void simulationLoop(SDL_Window *window);
static bool handleEvent(SDL_Event *event);

static bool
handleEvent(SDL_Event *event)
{
    bool isDone = false;
    
    switch(event->type) {
    case SDL_QUIT:
    case SDL_KEYDOWN:
        isDone = true;
        break;
    case SDL_WINDOWEVENT:
        if (event->window.event == SDL_WINDOWEVENT_RESIZED) {
            gltutReshape(event->window.data1, event->window.data2);
        }
        break;
    default:
        break;
    }

    return isDone;
}

static void
simulationLoop(SDL_Window *window)
{
	SDL_Event event;
	bool isDone = false;

    // @TODO Handle keyboard and mouse
    // @TODO Handle reshape
	while (!isDone) {
		while(SDL_PollEvent(&event) == 1) {
            isDone = handleEvent(&event);
		}
        gltutDisplay();
		SDL_GL_SwapWindow(window);
	}
}

int
main(int argc, char** argv)
{
    GltutDefaultSettings settings = GLTUT_DEFAULT_SETTINGS_INITIALIZER;
    SDL_GLContext mainContext;
    SDL_Window *mainWindow;
    int retval;
    const char *errorStr;

	(void) argc;
	(void) argv;

    gltutDefaultSettingsInit(&settings);

    retval = SDL_Init(SDL_INIT_VIDEO);
    assert(retval == 0);
    
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    mainWindow = SDL_CreateWindow(settings.windowTitle,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  settings.windowWidth,
                                  settings.windowHeight,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    assert(mainWindow != NULL);

    mainContext = SDL_GL_CreateContext(mainWindow);
    errorStr = SDL_GetError();
    assert(*errorStr == '\0');

    gltutPostRenderSystemInit();

    simulationLoop(mainWindow);

    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
