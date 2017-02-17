#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_ClearError();
    SDL_Window* mainWindow = SDL_CreateWindow("Determining GL Version",
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

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error Initializing GLEW: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLint major;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    GLint minor;
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    std::cout << "GL Renderer          : " << renderer << "\n"
              << "GL Vendor            : " << vendor << "\n"
              << "GL Version (string)  : " << version << "\n"
              << "GL Version (integer) : " << major << "." << minor << "\n"
              << "GLSL Version         : " << glslVersion << "\n";
    GLint numExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    if (numExtensions > 0) {
        std::cout << "GL Extensions (" << numExtensions << ")\n";
        for (int i = 0; i < numExtensions; ++i) {
            std::cout << "                       "
                      << glGetStringi(GL_EXTENSIONS, i) << "\n";
        }
    }
    
    SDL_GL_DeleteContext(mainContext);
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();

    return 0;
}
