# TODO(kreeger): Handle Angle build includes here.
ifeq ($(OS),Windows_NT)
    CCFLAGS += -D WIN32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
    endif
endif

all: gl_extensions half_float_tex_test

gl_extensions: gl_extensions.cc
	g++ gl_extensions.cc -lGLESv2 -lEGL -o gl_extensions

half_float_tex_test: half_float_tex_test.cc
	g++ half_float_tex_test.cc -lGLESv2 -lEGL -o half_float_tex_test

clean:
	rm -f *.o gl_extensions half_float_tex_test
