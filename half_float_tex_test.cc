/* #include <EGL/egl.h> */
/* #include <GLES2/gl2.h> */
#include <iostream>
#include <memory>
#include <string.h>

#include "egl_utils.h"

int main() {
  std::unique_ptr<EGLSession> session(EGLSession::Create());
  if (!session) {
    std::cerr << "Could not init EGL session" << std::endl;
    return 1;
  }

  // First - check if 'OES_texture_half_float' is enabled:
  PFNGLGETSTRINGPROC get_string =
      reinterpret_cast<PFNGLGETSTRINGPROC>(eglGetProcAddress("glGetString"));
  if (strstr(reinterpret_cast<const char*>(get_string(GL_EXTENSIONS)),
             "OES_texture_half_float") == nullptr) {
    std::cerr << "OES_texture_half_float is not supported!" << std::endl;
    return 1;
  }

  // Create 2 buffers:
  // vertex, index

  GLuint buffers[2];
  glGenBuffers(2, buffers);

  // Bind vertex buffer:
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  float coords[] = {-1.f, 1.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f, 0.f,
                    1.f,  1.f, 0.f, 1.f, 1.f, 1.f,  -1.f, 0.f, 1.f, 0.f};
  glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

  // Bind index buffer:
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
  uint16_t indices[] = {0, 1, 2, 2, 1, 3};
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // Create an off-screen frame buffer:
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);

  // Create two textures.
  GLuint textures[2];
  glGenTextures(2, textures);

  return 1;
}
