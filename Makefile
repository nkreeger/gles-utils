TARGET=gl_extensions

all: Makefile $(TARGET)

$(TARGET): gl_extensions.cc
	g++ -lGLESv2 -lEGL \
		-o $(TARGET) \
		gl_extensions.cc

clean:
	rm -f $(TARGET)
