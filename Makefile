TARGET=smoketest

all: Makefile $(TARGET)

$(TARGET): test.cc
	g++ -lGLESv2 -lEGL \
		-o $(TARGET) \
		test.cc

clean:
	rm -f $(TARGET)
