#include <iostream>
#include <memory>

#include "egl_utils.h"
/* #include <EGL/egl.h> */
/* #include <GLES2/gl2.h> */

int main() {
  std::unique_ptr<EGLSession> session(EGLSession::Create());
  if (!session) {
    std::cerr << "Could not init EGL session" << std::endl;
    return 1;
  }

  return 0;
}
