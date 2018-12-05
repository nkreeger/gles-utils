IFLAGS=
LDDFLAGS=

ifeq ($(OS),Windows_NT)
		# TODO(kreeger): Handle Angle build includes here.
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
				IFLAGS += -I../angle/include
				LDDFLAGS += -L../angle/out/Debug
    endif
endif

all: gl_extensions half_float_tex_test int_tex_test

gl_extensions: gl_extensions.cc
	g++ $(IFLAGS) gl_extensions.cc $(LDDFLAGS) -lGLESv2 -lEGL -o gl_extensions

half_float_tex_test: half_float_tex_test.cc
	g++ -g $(IFLAGS) half_float_tex_test.cc $(LDDFLAGS) -lGLESv2 -lEGL -o half_float_tex_test

int_tex_test: int_tex_test.cc
	g++ -g $(IFLAGS) int_tex_test.cc $(LDDFLAGS) -lGLESv2 -lEGL -o int_tex_test

clean:
	rm -f *.o gl_extensions half_float_tex_test
