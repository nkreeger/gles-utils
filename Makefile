TARGET=gl_extensions

all: Makefile $(TARGET)

$(TARGET): gl_extensions.cc
	g++ gl_extensions.cc -lGLESv2 -lEGL -o $(TARGET)

clean:
	rm -f $(TARGET)
