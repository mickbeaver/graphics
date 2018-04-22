#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "SDL.h"

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
static void		 print_sorted_gl_exts(void);
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

static const char *
get_compressed_format_name(GLenum format)
{
	switch (format) {
	case GL_ETC1_RGB8_OES: 
		return "GL_ETC1_RGB8_OES";
	case GL_PALETTE4_RGB8_OES:
		return "GL_PALETTE4_RGB8_OES";
	case GL_PALETTE4_RGBA8_OES:
		return "GL_PALETTE4_RGBA8_OES";
	case GL_PALETTE4_R5_G6_B5_OES:
		return "GL_PALETTE4_R5_G6_B5_OES";
	case GL_PALETTE4_RGBA4_OES:
		return "GL_PALETTE4_RGBA4_OES";
	case GL_PALETTE4_RGB5_A1_OES:
		return "GL_PALETTE4_RGB5_A1_OES";
	case GL_PALETTE8_RGB8_OES:
		return "GL_PALETTE8_RGB8_OES";
	case GL_PALETTE8_RGBA8_OES:
		return "GL_PALETTE8_RGBA8_OES";
	case GL_PALETTE8_R5_G6_B5_OES:
		return "GL_PALETTE8_R5_G6_B5_OES";
	case GL_PALETTE8_RGBA4_OES:
		return "GL_PALETTE8_RGBA4_OES";
	case GL_PALETTE8_RGB5_A1_OES:
		return "GL_PALETTE8_RGB5_A1_OES";
	default:
		break;
	};

	return "<name unknown>";
}

static void
print_compressed_texture_formats(void)
{
	GLint	numFormats;
	
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &numFormats);
	printf("GL_NUM_COMPRESSED_TEXTURE_FORMATS = %d\n", numFormats);
	
	if (numFormats > 0) {
		GLint *formats;

		formats = (GLint*)malloc(sizeof(GLint) * numFormats);
		glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, formats);
		for (int i = 0; i < numFormats; i++) {
			printf("\tformat %d = 0x%04X %s\n", i, formats[i], get_compressed_format_name(formats[i]));
		}

		free(formats);
	}
}

static void
print_sorted_gl_exts(void)
{
	size_t	 len_of_extensions_dup;
	size_t	 num_extensions = 0;
	char	*extensions_dup;
	char	*current_char;
	const GLubyte *gl_exts;
    
	gl_exts = glGetString(GL_EXTENSIONS);
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
		printf("There are %u extensions\n", (unsigned)num_extensions);
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
	} else {
		printf("GL_EXTENSIONS = %s\n", "<no extensions>");
	}

	free(extensions_dup);
}


static void
print_gl_float_range(GLenum pname, const char *name)
{
	GLfloat	results[2];
	
	glGetFloatv(pname, results);
	printf("%s = %f to %f\n", name, results[0], results[1]);
}

static void
print_gl_int(GLenum pname, const char *name)
{
	GLint	result;
	
	glGetIntegerv(pname, &result);
	printf("%s = %d\n", name, result);
}

static void
print_gl_string(GLenum pname, const char *name)
{
	const GLubyte	*result;
    
	result = glGetString(pname);
	printf("%s = %s\n", name, result != NULL ? (const char *)result : "<ERROR>");
}

#define PRINT_GL_FLOAT_RANGE(name) print_gl_float_range(name, #name)
#define PRINT_GL_INT(name) print_gl_int(name, #name)
#define PRINT_GL_STRING(name) print_gl_string(name, #name)

static void
print_gl_info(void)
{
	PRINT_GL_STRING(GL_VENDOR);
	PRINT_GL_STRING(GL_RENDERER);
	PRINT_GL_STRING(GL_VERSION);
	PRINT_GL_STRING(GL_SHADING_LANGUAGE_VERSION);
	print_sorted_gl_exts();

	PRINT_GL_INT(GL_RED_BITS);
	PRINT_GL_INT(GL_GREEN_BITS);
	PRINT_GL_INT(GL_BLUE_BITS);
	PRINT_GL_INT(GL_ALPHA_BITS);
	PRINT_GL_INT(GL_DEPTH_BITS);

	PRINT_GL_INT(GL_NUM_SHADER_BINARY_FORMATS);

	print_compressed_texture_formats();

	PRINT_GL_INT(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	PRINT_GL_INT(GL_MAX_CUBE_MAP_TEXTURE_SIZE);
	PRINT_GL_INT(GL_MAX_FRAGMENT_UNIFORM_VECTORS);
	PRINT_GL_INT(GL_MAX_RENDERBUFFER_SIZE);
	PRINT_GL_INT(GL_MAX_TEXTURE_IMAGE_UNITS);
	PRINT_GL_INT(GL_MAX_TEXTURE_SIZE);
	PRINT_GL_INT(GL_MAX_VARYING_VECTORS);
	PRINT_GL_INT(GL_MAX_VERTEX_ATTRIBS);
	PRINT_GL_INT(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS);
	PRINT_GL_INT(GL_MAX_VERTEX_UNIFORM_VECTORS);

	PRINT_GL_FLOAT_RANGE(GL_ALIASED_LINE_WIDTH_RANGE);
	PRINT_GL_FLOAT_RANGE(GL_ALIASED_POINT_SIZE_RANGE);
	PRINT_GL_FLOAT_RANGE(GL_DEPTH_RANGE);
}

#undef PRINT_GL_FLOAT_RANGE
#undef PRINT_GL_INT
#undef PRINT_GL_STRING

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
