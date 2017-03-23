#!/bin/sh
set -x
python3 generator.py -i gl.xml write-glsys --header ../opengl_4_shading_language_cookbook/glsys/include/glsys.generated.h --c-module ../opengl_4_shading_language_cookbook/glsys/src/glsys.generated.c --feature GL_VERSION_4_3 --lib-feature GL_VERSION_1_1

