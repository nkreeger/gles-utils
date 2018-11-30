/* #include <EGL/egl.h> */
/* #include <GLES2/gl2.h> */
#include <string.h>
#include <iostream>
#include <memory>

// #include <GLES2/gl32.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "egl_utils.h"
#include "gles_utils.h"
#include "half_float_utils.h"

void test_texture(GLuint framebuffer, GLuint *texture) {
  // Create a 1x1 texture for now:
  /* half-float: R32F / RED / GL_HALF_FLOAT */
  // create_texture_2d(GL_R16F, GL_RED, GL_HALF_FLOAT, texture);
  create_texture_2d(GL_R16F_EXT, GL_RED_EXT, GL_HALF_FLOAT_OES, texture);

  // Bind test values:
  glBindTexture(GL_TEXTURE_2D, *texture);
  // uint16_t values[] = {Float16Compressor::compress(1.5f)};
  float values[] = {1.5f};
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RED_EXT, GL_FLOAT, values);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Now bind framebuffer and read values
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         *texture, 0);

  glViewport(0, 0, 1, 1);
  glScissor(0, 0, 1, 1);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

  // void *buffer = malloc(sizeof(uint16_t) * 1);
  void *buffer = malloc(sizeof(float) * 1);
  glReadPixels(0, 0, 1, 1, GL_RED_EXT, GL_FLOAT, buffer);

  // float value =
  //     Float16Compressor::decompress(static_cast<uint16_t *>(buffer)[0]);
  float value = static_cast<float*>(buffer)[0];
  std::cerr << "texture value: " << value << " (should be 1.5)" << std::endl;

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

  // Setup
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_DITHER);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glDisable(GL_SAMPLE_COVERAGE);
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

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

  GLuint texture;
  test_texture(framebuffer, &texture);

  /* // Create and bind the fragment shader: */
  /* GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER); */
  /* set_shader_src(fragment_shader, "fragment_shader.glsl"); */
  /* glCompileShader(fragment_shader); */
  /* check_for_errors(); */

  /* // Create and bind the vertex shader: */
  /* GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER); */
  /* set_shader_src(vertex_shader, "vertex_shader.glsl"); */
  /* glCompileShader(vertex_shader); */
  /* check_for_errors(); */

  // Create program and attach shaders
  /* GLuint program = glCreateProgram(); */
  /* glAttachShader(program, vertex_shader); */
  /* glAttachShader(program, fragment_shader); */
  /* glLinkProgram(program); */
  /* check_for_errors(); */

  // Ensure everything is good:

  return 0;
}
