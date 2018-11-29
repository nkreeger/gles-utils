#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <iostream>
#include <memory>
#include <string>

#include "egl_utils.h"

void LogExtensions(const char* extensions_name, const char* extensions) {
  if (!extensions) {
    std::cerr << "Null extensions string passed for : " << extensions_name
              << std::endl;
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
  std::cout << s << std::endl;
  std::cout << "-------------------------" << std::endl << std::endl;
}

int main() {
  LogExtensions("EGL_NO_DISPLAY_EXTENSIONS",
                eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS));

  std::unique_ptr<EGLSession> session(EGLSession::Create());
  if (!session) {
    std::cerr << "Could not init EGL session" << std::endl;
    return 1;
  }

  PFNGLGETSTRINGPROC get_string =
      reinterpret_cast<PFNGLGETSTRINGPROC>(eglGetProcAddress("glGetString"));

  LogExtensions("GL_EXTENSIONS",
                reinterpret_cast<const char*>(get_string(GL_EXTENSIONS)));

  LogExtensions("GL_VERSION",
                reinterpret_cast<const char*>(get_string(GL_VERSION)));

  LogExtensions("GL_RENDERER",
                reinterpret_cast<const char*>(get_string(GL_RENDERER)));

  LogExtensions("GL_VENDOR",
                reinterpret_cast<const char*>(get_string(GL_VENDOR)));

  return 0;
}
