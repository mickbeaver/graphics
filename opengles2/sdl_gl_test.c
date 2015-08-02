#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_assert.h"
#include "glsys.h"

#define sdl_print_error_and_exit(user_message) sdl_print_error_and_exit_func(SDL_FUNCTION, __LINE__, user_message)
#ifdef NDEBUG
#define sdl_check_error() sdl_check_and_clear_error_func(SDL_FUNCTION, __LINE__)
#else
#define sdl_check_error() (void)0
#endif


static void
sdl_print_error_and_exit_func(const char *function_name, int line, const char *user_message)
{
	printf("%s()[%d]: SDL Error: %s\n", function_name, line, SDL_GetError());
	if (user_message != NULL)
		printf("%s()[%d]: User message: %s\n", function_name, line, user_message);
	SDL_Quit();
	exit(EXIT_FAILURE);
}

#ifdef NDEBUG
static void
sdl_check_and_clear_error_func(const char *function_name, int line)
{
	const char	*error;

	error = SDL_GetError();
	if (*error != '\0') {
		printf("%s()[%d]: SDL Error: %s\n", function_name, line, error);
		SDL_ClearError();
	}
}
#endif

int
main(int argc, char *argv[])
{
	SDL_Window *mainwindow;
	SDL_GLContext maincontext;

	(void) argc;
	(void) argv;
 
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		sdl_print_error_and_exit("Unable to initialize SDL");
  
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
 
	mainwindow = SDL_CreateWindow("Yeah buddy",
	    SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED,
	    512,
	    512,
	    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (mainwindow == NULL)
		sdl_print_error_and_exit("SDL_CreateWindow() failed!");
	sdl_check_error();
 
	maincontext = SDL_GL_CreateContext(mainwindow);
	sdl_check_error();
 
	SDL_GL_SetSwapInterval(1);
 
	glClearColor( 1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mainwindow);
	SDL_Delay(2000);
 
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mainwindow);
	SDL_Delay(2000);
 
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mainwindow);
	SDL_Delay(2000);
 
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();
 
	return (0);
}
