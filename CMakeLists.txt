cmake_minimum_required(VERSION 2.8.12)

project(graphics C)

enable_testing()

if(UNIX)

	include_directories(/usr/local/include)
	link_directories(/usr/local/lib)
	if(Clang STREQUAL ${CMAKE_C_COMPILER_ID})
		add_definitions(
			-D_POSIX_C_SOURCE=200809L
			-D_XOPEN_SOURCE=700
			-Werror
			-Weverything
			-Wno-documentation
			-Wno-padded
			-pedantic-errors
			-std=c99
			)
	elseif(GNU STREQUAL ${CMAKE_C_COMPILER_ID})
		add_definitions(
			-D_POSIX_C_SOURCE=200809L
			-D_XOPEN_SOURCE=700
			-Wall
			-Werror
			-Wextra
			-pedantic-errors
			-std=c99
			)
	else()
		message(FATAL_ERROR "Unknown compiler. Please add your identifier and flags")
	endif()
else()
	message(FATAL_ERROR "Only Unix systems are supported.")
endif()

add_subdirectory(3dmath)
add_subdirectory(opengles2)

