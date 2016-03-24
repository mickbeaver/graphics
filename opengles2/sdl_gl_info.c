#include <GLES2/gl2.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "vector.h"

#define sdl_print_error_and_exit(user_message) sdl_print_error_and_exit_func(__FILE__, __LINE__, user_message)
#ifdef NDEBUG
#define sdl_check_error() (void) 0
#else
#define sdl_check_error() sdl_check_and_clear_error_func(SDL_FUNCTION, __LINE__)
#endif

static void		 sdl_print_error_and_exit_func(const char *_function_name, int _line, const char *_user_message);
static void		 sdl_print_error_and_exit_func(const char *_function_name, int _line, const char *_user_mesage);
static void		 print_sdl_info(void);
static int		 strings_qsort_cmp_func(void const *, void const *);
static void		 print_gl_info(void);
static void		 print_sorted_gl_exts(const GLubyte *);
static char		*local_strdup(const char *_original);

static char *
local_strdup(const char *original)
{
	size_t	 length;
	char	*copy_to_return;

	length = strlen(original);
	copy_to_return = (char *)malloc(length + 1);
	strcpy(copy_to_return, original);

	return copy_to_return;
}

static void
sdl_print_error_and_exit_func(const char *function_name, int line, const char *user_message)
{
	printf("%s()[%d]: SDL Error: %s\n", function_name, line, SDL_GetError());
	if (user_message != NULL)
		printf("%s()[%d]: User message: %s\n", function_name, line, user_message);
	SDL_Quit();
	exit(EXIT_FAILURE);
}
 
 
static void
sdl_check_and_clear_error_func(const char *function_name, int line)
{
	const char *error;

	error = SDL_GetError();
	if (*error != '\0') {
		printf("%s()[%d]: SDL Error: %s\n", function_name, line, error);
		SDL_ClearError();
	}
}

static void
print_sdl_info(void)
{
}

static int
strings_qsort_cmp_func(void const *arg1, void const *arg2)
{
	char const * const * const ptr_to_string1 = (char const * const *)arg1;
	char const * const * const ptr_to_string2 = (char const * const *)arg2;
	char const * const string1 = *ptr_to_string1;
	char const * const string2 = *ptr_to_string2;

	return (strcmp(string1, string2));
}

static void
print_sorted_gl_exts(const GLubyte *gl_exts)
{
	size_t	 len_of_extensions_dup;
	size_t	 num_extensions = 0;
	char	*extensions_dup;
	char	*current_char;
        
	extensions_dup = local_strdup((char const *)gl_exts);
	len_of_extensions_dup = strlen(extensions_dup);
	current_char = extensions_dup;

	/* account for the extensions string ending in a space */
	if (len_of_extensions_dup > 0 &&
	    extensions_dup[len_of_extensions_dup - 1] != ' ')
		++num_extensions;

	while (*current_char != '\0') {
		if (isspace((int)*current_char))
			++num_extensions;
		++current_char;
	}

	if (num_extensions > 0) {
		size_t	current_extension_index;
		char	*current_extension, **extensions;
		const char *extension_printf_padding;

		extensions = (char **)malloc(sizeof(char *) * num_extensions);
		printf("There are %zu extensions\n", num_extensions);
		current_extension = strtok(extensions_dup, " ");

		current_extension_index = 0;
		while (current_extension != NULL) {
			extensions[current_extension_index] = current_extension;
			++current_extension_index;
			current_extension = strtok(NULL, " ");
		}
		qsort(extensions, num_extensions, sizeof(char *), strings_qsort_cmp_func);

		extension_printf_padding = "GL_EXTENSIONS = ";
		for (current_extension_index = 0; current_extension_index < num_extensions; ++current_extension_index) {
			printf("%s%s\n", extension_printf_padding, extensions[current_extension_index]);
			extension_printf_padding = "                ";
		}
            
		free(extensions);
	} else
		printf("GL_EXTENSIONS = %s\n", "<no extensions>");

	free(extensions_dup);
}

static void
print_gl_info(void)
{
	GLint		 get_int_result;
	GLfloat		 get_float_result[2];
	const GLubyte	*get_string_result;
    
	get_string_result = glGetString(GL_VENDOR);
	printf("GL_VENDOR = %s\n", get_string_result != 0 ? (const char *)get_string_result : "<ERROR>");
	get_string_result = glGetString(GL_RENDERER);
	printf("GL_RENDERER = %s\n", get_string_result != 0 ? (const char *)get_string_result : "<ERROR>");
	get_string_result = glGetString(GL_VERSION);
	printf("GL_VERSION = %s\n", get_string_result != 0 ? (const char *)get_string_result : "<ERROR>");
	get_string_result = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("GL_SHADING_LANGUAGE_VERSION = %s\n", get_string_result != 0 ? (const char *)get_string_result : "<ERROR>");
	get_string_result = glGetString(GL_EXTENSIONS);
	if (get_string_result != 0)
		print_sorted_gl_exts(get_string_result);
	else
		printf("GL_EXTENSIONS = %s\n", "<ERROR>");
	glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS, &get_int_result);
	printf("GL_NUM_SHADER_BINARY_FORMATS = %d\n", get_int_result);

	glGetIntegerv(GL_MAX_VARYING_VECTORS, &get_int_result);
	printf("GL_MAX_VARYING_VECTORS = %d\n", get_int_result);
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &get_int_result);
	printf("GL_MAX_VERTEX_ATTRIBS = %d\n", get_int_result);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &get_int_result);
	printf("GL_MAX_VERTEX_UNIFORM_VECTORS = %d\n", get_int_result);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &get_int_result);
	printf("GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d\n", get_int_result);
	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, get_float_result);
	printf("GL_ALIASED_LINE_WIDTH_RANGE = %f to %f\n", get_float_result[0], get_float_result[1]);
	glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, get_float_result);
	printf("GL_ALIASED_POINT_SIZE_RANGE = %f to %f\n", get_float_result[0], get_float_result[1]);
}

int
main(int argc, char *argv[])
{
	int		 retval;
	SDL_Window	*main_window;
	SDL_GLContext	 main_context;

	(void) argc;
	(void) argv;

	retval = SDL_Init(SDL_INIT_VIDEO);
	if (retval < 0)
		sdl_print_error_and_exit("Unable to initialize SDL");
  
	retval = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	retval = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	retval = SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	retval = SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
 
	main_window = SDL_CreateWindow("Yeah buddy",
	    SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED,
	    512,
	    512,
	    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (main_window == NULL)
		sdl_print_error_and_exit("SDL_CreateWindow() failed!");
	sdl_check_error();
 
	main_context = SDL_GL_CreateContext(main_window);
	sdl_check_error();

	retval = SDL_GL_SetSwapInterval(1); /* sync with vert refresh */

	print_sdl_info();
	print_gl_info();

	SDL_GL_DeleteContext(main_context);
	SDL_DestroyWindow(main_window);
	SDL_Quit();
 
	return 0;
}
