#include <iostream>
#include <string>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

void LogExtensions(const char* extensions_name, const char* extensions) {
  if (!extensions) {
    std::cerr << "Null extensions string passed for : " << extensions_name << std::endl;
    return;
  }

  std::string s(extensions);
  std::string delim = " ";
  size_t pos = 0;
  std::string token;
  std::cout << "---- " << extensions_name << "----" << std::endl;
  while ((pos = s.find(delim)) != std::string::npos) {
    token = s.substr(0, pos);
    std::cout << token << std::endl;
    s.erase(0, pos + delim.length());
  }
  std::cout << s;
  std::cout << "-------------------------" << std::endl;
}

int main() {
  EGLDisplay display;
  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (display == EGL_NO_DISPLAY) {
    std::cerr << "No display" << std::endl;
    return 1;
  }

  if (!eglInitialize(display, nullptr, nullptr)) {
    std::cerr << "Could not initialize display" << std::endl;
    return 1;
  }

  eglBindAPI(EGL_OPENGL_ES_API);
  if (eglGetError() != EGL_SUCCESS) {
    std::cerr << "Failed to set OpenGL ES API" << std::endl;
    return 1;
  }

  EGLint attrib_list[] = {EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                          EGL_RED_SIZE,     8,
                          EGL_GREEN_SIZE,   8,
                          EGL_BLUE_SIZE,    8,
                          EGL_ALPHA_SIZE,   8,
                          EGL_DEPTH_SIZE,   24,
                          EGL_STENCIL_SIZE, 8,
                          EGL_NONE};

  EGLint num_config;
  EGLConfig config;
  if (!eglChooseConfig(display, attrib_list, &config, 1, &num_config)) {
    std::cerr << "Failed creating a config" << std::endl;
    return 1;
  }

  EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs);
  if (context == EGL_NO_CONTEXT) {
    std::cerr << "Could not create context" << std::endl;
    return 1;
  }

  EGLint surface_attribs[] = {EGL_LARGEST_PBUFFER, EGL_TRUE, EGL_NONE};
  EGLSurface surface = eglCreatePbufferSurface(display, config, surface_attribs);
  if (surface == EGL_NO_SURFACE) {
    std::cerr << "Could not create surface" << std::endl;
    return 1;
  }
  if (!eglMakeCurrent(display, surface, surface, context)) {
    std::cerr << "Could not make context current" << std::endl;
    return 1;
  }

  PFNGLGETSTRINGPROC get_string =
      reinterpret_cast<PFNGLGETSTRINGPROC>(eglGetProcAddress("glGetString"));

  LogExtensions("GL_EXTENSIONS",
                reinterpret_cast<const char*>(get_string(GL_EXTENSIONS)));

  eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroyContext(display, context);
  eglDestroySurface(display, surface);

  return 0;
}
