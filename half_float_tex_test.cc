#include <string.h>
#include <iostream>
#include <memory>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "egl_utils.h"
#include "gles_utils.h"
#include "half_float_utils.h"

void test_texture(GLuint framebuffer, GLuint *texture) {
  // Create a 1x1 texture for now:
  create_texture_2d(GL_RGBA, GL_RGBA, GL_HALF_FLOAT_OES, texture);

  // Bind test values:
  glBindTexture(GL_TEXTURE_2D, *texture);
  float values[] = {1.5f, 0.0f, 0.0f, 0.0f};
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_FLOAT, values);

  // Now bind framebuffer and read values
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         *texture, 0);
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    fprintf(stderr, "Texture failed to attach to framebuffer: 0x%X\n", status);
  }

  glViewport(0, 0, 1, 1);
  glScissor(0, 0, 1, 1);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

  void *buffer = malloc(sizeof(float) * 4);
  glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, buffer);

  std::cerr << "texture value: " << static_cast<float *>(buffer)[0]
            << " (should be 1.5)" << std::endl;
  std::cerr << "texture value: " << static_cast<float *>(buffer)[1]
            << " (should be 0)" << std::endl;
  std::cerr << "texture value: " << static_cast<float *>(buffer)[2]
            << " (should be 0)" << std::endl;
  std::cerr << "texture value: " << static_cast<float *>(buffer)[3]
            << " (should be 0)" << std::endl;

  free(buffer);
}

int main() {
  std::unique_ptr<EGLSession> session(EGLSession::Create());
  if (!session) {
    std::cerr << "Could not init EGL session" << std::endl;
    return 1;
  }

  // First - check if 'OES_texture_half_float' is enabled:
  PFNGLGETSTRINGPROC get_string =
      reinterpret_cast<PFNGLGETSTRINGPROC>(eglGetProcAddress("glGetString"));
  if (strstr(reinterpret_cast<const char *>(get_string(GL_EXTENSIONS)),
             "OES_texture_half_float") == nullptr) {
    std::cerr << "OES_texture_half_float is not supported!" << std::endl;
    return 1;
  }

  GLuint vertex_buffer;
  GLuint index_buffer;
  GLuint framebuffer;
  bootstrap_env(&vertex_buffer, &index_buffer, &framebuffer);

  GLuint texture;
  test_texture(framebuffer, &texture);

  return 0;
}
