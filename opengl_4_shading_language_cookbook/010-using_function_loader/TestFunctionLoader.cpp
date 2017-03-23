#include <cstdlib>
#include <iostream>

#include <SDL2/SDL.h>
#include "glsys.h"

#define WINDOW_SIZE 640

int
main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    int retval = SDL_Init(SDL_INIT_VIDEO);
    if (retval != 0) {
        std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    Uint32 glContextFlags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
#ifdef DEBUG
    glContextFlags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glContextFlags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_ClearError();
    SDL_Window* mainWindow = SDL_CreateWindow("Using Function Loader",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              WINDOW_SIZE,
                                              WINDOW_SIZE,
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);
    if (mainWindow == NULL) {
        std::cerr << "SDL_CreateWindow() failed: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
    if (mainContext == NULL) {
        std::cerr << "SDL_GL_CreateContext() failedd: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    retval = glsysLoadFunctions((glsysFunctionLoader)SDL_GL_GetProcAddress);
    if (retval == 0) {
        std::cout << "glsysLoadFunctions() was successful!\n";
    } else {
        std::cerr << "Error Initializing GL functions" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
