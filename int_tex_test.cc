#include <string.h>
#include <iostream>
#include <memory>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "egl_utils.h"
#include "gles_utils.h"
#include "half_float_utils.h"

void test_texture(GLuint framebuffer, GLuint *texture) {
  //
  // TODO - write me.
  //
  create_texture_2d(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, texture);

  glBindTexture(GL_TEXTURE_2D, *texture);
  uint8_t values[] = {2, 0, 0, 0};
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                  values);

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

  void *buffer = malloc(sizeof(uint8_t) * 4);
  glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

  fprintf(stderr, "texture value: %d (should be 2)\n",
          static_cast<uint8_t *>(buffer)[0]);
  fprintf(stderr, "texture value: %d (should be 0)\n",
          static_cast<uint8_t *>(buffer)[1]);
  fprintf(stderr, "texture value: %d (should be 0)\n",
          static_cast<uint8_t *>(buffer)[2]);
  fprintf(stderr, "texture value: %d (should be 0)\n",
          static_cast<uint8_t *>(buffer)[3]);

  free(buffer);
}

int main() {
  std::unique_ptr<EGLSession> session(EGLSession::Create());
  if (!session) {
    std::cerr << "Could not init EGL session" << std::endl;
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
