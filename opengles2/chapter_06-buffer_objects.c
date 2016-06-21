#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "glsys.h"


#define sdl_print_error_and_exit(user_message) sdl_print_error_and_exit_func(__FILE__, __LINE__, user_message)
#ifdef NDEBUG
#define sdl_check_error() (void) 0
#else
#define sdl_check_error() sdl_check_and_clear_error_func(__FILE__, __LINE__)
#endif

#define VERTEX_POS_INDEX 0
#define VERTEX_POS_SIZE	 3
#define VERTEX_COLOR_INDEX 1
#define VERTEX_COLOR_SIZE  3

#define USE_INTERLEAVED_MESH
#ifdef USE_INTERLEAVED_MESH
static GLfloat triangle_interleaved_mesh[] = {
    0.0f, 0.5f, 0.0f,
    1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.0f, 1.0f};
#else
static GLfloat triangle_vertex_pos[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f};
static GLfloat triangle_vertex_color[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f};
#endif

static GLushort triangle_indices[] = {0, 1, 2};

static void	 draw(void);
static void	 simulation_loop(SDL_Window *window);
static void	 setup_buffers(void);
static void	 sdl_print_error_and_exit_func(const char *function_name, int line, const char *user_message);
static void	 sdl_check_and_clear_error_func(const char *function_name, int line);
static GLchar	*load_text_file_into_string(const char *filename);
static GLuint	 load_shader(const GLenum shader_type, const char *filename);
static GLuint	 load_program(const char *vertex_shader_filename, const char *fragment_shader_filename);

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
	const char	*error;

	error = SDL_GetError();
	if (*error != '\0') {
		printf("%s()[%d]: SDL Error: %s\n", function_name, line, error);
		SDL_ClearError();
	}
}

static GLchar *
load_text_file_into_string(const char *filename)
{
	long	 file_length;
	size_t	 bytes_read;
	int	 ret = 0;
	FILE	*input_obj;
	char	*buffer = NULL;

	input_obj = fopen(filename, "rb");
	assert(input_obj != NULL);
	ret = fseek(input_obj, 0, SEEK_END);
	assert(ret == 0);
	file_length = ftell(input_obj);
	assert(file_length > 0);
	ret = fseek(input_obj, 0, SEEK_SET);
	assert(ret == 0);
	buffer = (GLchar *)malloc((unsigned long)file_length + 1 /*NUL*/);
    
	bytes_read = fread(buffer, 1, (unsigned long)file_length, input_obj);
	if (bytes_read == 0 && ferror(input_obj) != 0)
		printf("Error reading from %s. errno is %d\n", filename, errno);
	fclose(input_obj);
	buffer[file_length] = '\0';
	return buffer;
}

static GLuint
load_shader(const GLenum shader_type, const char *filename)
{
	GLint		 compile_status = 0, info_log_length = 0;
	GLuint		 shader_handle;
	const GLchar	*file_buffer;

	shader_handle = glCreateShader(shader_type);
	assert(shader_handle != 0);

	file_buffer = load_text_file_into_string(filename);
	glShaderSource(shader_handle, 1, &file_buffer, NULL);

	glCompileShader(shader_handle);
	glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		printf("Error compiling %s\n", filename);
	}
	glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 1) {
		char *info_log_buffer = (char *)malloc((GLuint)info_log_length);
		glGetShaderInfoLog(shader_handle, info_log_length, NULL, info_log_buffer);
		printf("Info log for shader %s\n", filename);
		puts(info_log_buffer);
		free(info_log_buffer);
	}
	assert(compile_status == GL_TRUE);

	return shader_handle;
}
 
static GLuint
load_program(const char *vertex_shader_filename, const char *fragment_shader_filename)
{
	GLint	info_log_length = 0, link_status = 0;
	GLuint	fragment_shader, vertex_shader, program_handle;

	program_handle = glCreateProgram();
	assert(program_handle != 0);
	vertex_shader = load_shader(GL_VERTEX_SHADER, vertex_shader_filename);;
	assert(vertex_shader != 0);
	fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragment_shader_filename);
	assert(fragment_shader != 0);

	glAttachShader(program_handle, vertex_shader);
	glAttachShader(program_handle, fragment_shader);
	glBindAttribLocation(program_handle, VERTEX_POS_INDEX, "a_position");
	glBindAttribLocation(program_handle, VERTEX_COLOR_INDEX, "a_color");
	glLinkProgram(program_handle);
	glGetProgramiv(program_handle, GL_LINK_STATUS, &link_status);
	glGetProgramiv(program_handle, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 1) {
		char *info_log_buffer = (char *)malloc((GLuint)info_log_length);
		glGetProgramInfoLog(program_handle, info_log_length, NULL, info_log_buffer);
		printf("\n\nInfo log for program %u (vert: %s frag %s)\n", program_handle, vertex_shader_filename, fragment_shader_filename);
		printf("%s\n", info_log_buffer);
		free(info_log_buffer);
	}

	assert(link_status == GL_TRUE);
	return program_handle;
}

static void
draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, sizeof(triangle_indices) / sizeof(triangle_indices[0]), GL_UNSIGNED_SHORT, (const void *)0);
}

static void
simulation_loop(SDL_Window *window)
{
	SDL_Event	event;
	GLboolean	is_done = GL_FALSE;

	while (!is_done) {
		while(SDL_PollEvent(&event) == 1) {
			if (event.type == SDL_QUIT)
				is_done = GL_TRUE;
			else if (event.type == SDL_KEYDOWN)
				is_done = GL_TRUE;
		}
		draw();
		SDL_GL_SwapWindow(window);
	}
}

static void
setup_buffers(void)
{
	const GLsizei vertex_stride = sizeof(GLfloat) * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE);
	intptr_t offset_into_buffer = 0;
	GLuint buffer_objects[2];

	glGenBuffers(2, buffer_objects);

	glBindBuffer(GL_ARRAY_BUFFER, buffer_objects[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_interleaved_mesh), triangle_interleaved_mesh, GL_STATIC_DRAW);
	
	glVertexAttribPointer(VERTEX_POS_INDEX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, vertex_stride, (const void *)offset_into_buffer);
	offset_into_buffer += sizeof(GLfloat) * VERTEX_POS_SIZE;
	glEnableVertexAttribArray(VERTEX_POS_INDEX);
	glVertexAttribPointer(VERTEX_COLOR_INDEX, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, vertex_stride, (const void *)offset_into_buffer);
	glEnableVertexAttribArray(VERTEX_COLOR_INDEX);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_objects[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);
}

int
main(int argc, char *argv[])
{
	SDL_GLContext	 main_context;
	GLuint		 shader_program;
	SDL_Window	*main_window;

	(void) argc;
	(void) argv;
 
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		sdl_print_error_and_exit("Unable to initialize SDL");
	}
  
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	main_window = SDL_CreateWindow("Yeah buddy",
	    SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED,
	    512,
	    512,
	    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (main_window == NULL) {
		sdl_print_error_and_exit("SDL_CreateWindow() failed!");
	}
	sdl_check_error();
 
	main_context = SDL_GL_CreateContext(main_window);
	sdl_check_error();

    glsysInit();

	shader_program = load_program("shaders/chapter_06.vert", "shaders/chapter_06.frag");
	glUseProgram(shader_program);

	setup_buffers();
	 
	SDL_GL_SetSwapInterval(1); /* Sync with the refresh rate */

	simulation_loop(main_window);
 
	SDL_GL_DeleteContext(main_context);
	SDL_DestroyWindow(main_window);
	SDL_Quit();
 
	return 0;
}
