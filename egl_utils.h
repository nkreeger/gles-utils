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
    eglDestroyContext(display_, context_);
    eglDestroySurface(display_, surface_);
  }

  EGLContext context_;
  EGLDisplay display_;
  EGLConfig config_;
  EGLSurface surface_;

 private:
  bool init_success;

  EGLSession() : init_success(false) {
    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display_ == EGL_NO_DISPLAY) {
      std::cerr << "No display" << std::endl;
      return;
    }

    if (!eglInitialize(display_, nullptr, nullptr)) {
      std::cerr << "Could not initialize display" << std::endl;
      return;
    }

    eglBindAPI(EGL_OPENGL_ES_API);
    if (eglGetError() != EGL_SUCCESS) {
      std::cerr << "Failed to set OpenGL ES API" << std::endl;
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
    if (!eglChooseConfig(display_, attrib_list, &config_, 1, &num_config)) {
      std::cerr << "Failed creating an EGL config" << std::endl;
      return;
    }

    EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context_ =
        eglCreateContext(display_, config_, EGL_NO_CONTEXT, context_attribs);
    if (context_ == EGL_NO_CONTEXT) {
      std::cerr << "Could not create context" << std::endl;
      return;
    }

    EGLint surface_attribs[] = {EGL_LARGEST_PBUFFER, EGL_TRUE, EGL_NONE};
    surface_ = eglCreatePbufferSurface(display_, config_, surface_attribs);
    if (surface_ == EGL_NO_SURFACE) {
      std::cerr << "Could not create surface" << std::endl;
      return;
    }
    if (!eglMakeCurrent(display_, surface_, surface_, context_)) {
      std::cerr << "Could not make context current" << std::endl;
      return;
    }

    // EGL successfully setup:
    init_success = true;
  }
};

#endif  // EGL_UTILS_H_
