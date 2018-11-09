#ifndef EGL_UTILS_H_
#define EGL_UTILS_H_

#include <EGL/egl.h>
#include <GLES2/gl2.h>

class EGLSession {
 public:
  static EGLSession* Create() {
    EGLSession* session = new EGLSession();
    if (session->init_success) {
      return session;
    }
    delete session;
    return nullptr;
  }

  ~EGLSession() {
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
  }

  EGLContext context;
  EGLDisplay display;
  EGLConfig config;
  EGLSurface surface;

 private:
  bool init_success;

  EGLSession() : init_success(false) {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
      std::cerr << "No display" << std::endl;
      return;
    }

    if (!eglInitialize(display, nullptr, nullptr)) {
      std::cerr << "Could not initialize display" << std::endl;
      return;
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
    if (!eglChooseConfig(display, attrib_list, &config, 1, &num_config)) {
      std::cerr << "Failed creating an EGL config" << std::endl;
      return;
    }

    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context =
        eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs);
    if (context == EGL_NO_CONTEXT) {
      std::cerr << "Could not create context" << std::endl;
      return;
    }

    eglBindAPI(EGL_OPENGL_ES_API);
    if (eglGetError() != EGL_SUCCESS) {
      std::cerr << "Failed to set OpenGL ES API" << std::endl;
      return;
    }

    EGLint surface_attribs[] = {EGL_LARGEST_PBUFFER, EGL_TRUE, EGL_NONE};
    EGLSurface surface =
        eglCreatePbufferSurface(display, config, surface_attribs);
    if (surface == EGL_NO_SURFACE) {
      std::cerr << "Could not create surface" << std::endl;
      return;
    }
    if (!eglMakeCurrent(display, surface, surface, context)) {
      std::cerr << "Could not make context current" << std::endl;
      return;
    }

    // EGL successfully setup:
    init_success = true;
  }
};

#endif  // EGL_UTILS_H_
