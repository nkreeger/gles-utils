/* #include <EGL/egl.h> */
/* #include <GLES2/gl2.h> */
#include <string.h>
#include <iostream>
#include <fstream>
#include <memory>

#include <GLES3/gl32.h>

#include "egl_utils.h"

void set_shader_src(GLuint shader, const char* file_name) {
  std::string src;
  std::ifstream file(file_name);
  file.seekg(0, std::ios::end);
  src.reserve(file.tellg());
  file.seekg(0, std::ios::beg);

  src.assign(
      (std::istreambuf_iterator<char>(file)),
      std::istreambuf_iterator<char>());

  const char* codes[] = {src.c_str()};
  GLint length = src.size();

  glShaderSource(shader, 1, codes, &length);
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
  if (strstr(reinterpret_cast<const char*>(get_string(GL_EXTENSIONS)),
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

  // Create texture.
  GLuint texture;
  glGenTextures(1, &texture);

  // Bind texture and configure
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, 2, 2, 0, GL_RED, GL_HALF_FLOAT,
               nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Create and bind the fragment shader:
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  set_shader_src(fragment_shader, "fragment_shader.glsl");
  glCompileShader(fragment_shader);

  // Create and bind the vertex shader:
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  set_shader_src(vertex_shader, "vertex_shader.glsl");
  glCompileShader(vertex_shader);

  return 1;
}
