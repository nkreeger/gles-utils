/* #include <EGL/egl.h> */
/* #include <GLES2/gl2.h> */
#include <string.h>
#include <fstream>
#include <iostream>
#include <memory>

#include <GLES3/gl32.h>

#include "egl_utils.h"

bool check_for_errors() {
  GLenum error;
  bool had_error = false;
  while ((error = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "HAS ERRORS()\n");
    switch (error) {
      case GL_INVALID_ENUM:
        fprintf(stderr, "Found unchecked GL error: GL_INVALID_ENUM\n");
        break;
      case GL_INVALID_VALUE:
        fprintf(stderr, "Found unchecked GL error: GL_INVALID_VALUE\n");
        break;
      case GL_INVALID_OPERATION:
        fprintf(stderr, "Found unchecked GL error: GL_INVALID_OPERATION\n");
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        fprintf(stderr,
                "Found unchecked GL error: GL_INVALID_FRAMEBUFFER_OPERATION\n");
        break;
      case GL_OUT_OF_MEMORY:
        fprintf(stderr, "Found unchecked GL error: GL_OUT_OF_MEMORY\n");
        break;
      default:
        fprintf(stderr, "Found unchecked GL error: UNKNOWN ERROR\n");
        break;
    }
  }
  return had_error;
}

void set_shader_src(GLuint shader, const char* file_name) {
  std::string src;
  std::ifstream file(file_name);
  file.seekg(0, std::ios::end);
  src.reserve(file.tellg());
  file.seekg(0, std::ios::beg);

  src.assign((std::istreambuf_iterator<char>(file)),
             std::istreambuf_iterator<char>());

  const char* codes[] = {src.c_str()};
  GLint length = src.size();
  glShaderSource(shader, 1, codes, &length);
  check_for_errors();
}

// 1.) TODO Break this into:
//
// Init()
// Upload()
// Add()
// ---> Run init(), add().

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
  check_for_errors();

  // Create 2 buffers:
  // vertex, index

  GLuint buffers[2];
  glGenBuffers(2, buffers);
  check_for_errors();

  // Bind vertex buffer:
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  float coords[] = {-1.f, 1.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f, 0.f,
                    1.f,  1.f, 0.f, 1.f, 1.f, 1.f,  -1.f, 0.f, 1.f, 0.f};
  glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
  check_for_errors();

  // Bind index buffer:
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
  uint16_t indices[] = {0, 1, 2, 2, 1, 3};
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  check_for_errors();

  // Create an off-screen frame buffer:
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  check_for_errors();

  // Create texture.
  GLuint texture;
  glGenTextures(1, &texture);
  check_for_errors();

  // Bind texture and configure
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, 2, 2, 0, GL_RED, GL_HALF_FLOAT,
               nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
  check_for_errors();

  // Create and bind the fragment shader:
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  set_shader_src(fragment_shader, "fragment_shader.glsl");
  glCompileShader(fragment_shader);
  check_for_errors();

  // Create and bind the vertex shader:
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  set_shader_src(vertex_shader, "vertex_shader.glsl");
  glCompileShader(vertex_shader);
  check_for_errors();

  // Create program and attach shaders
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  check_for_errors();

  // Ensure everything is good:
  GLint param;
  glGetProgramiv(program, GL_LINK_STATUS, &param);
  if (!param) {
    // TODO(kreeger): Get the log and report
    std::cerr << "Could not create the program!" << std::endl;
    return 1;
  }

  // TODO(kreeger): Remove this soon
  std::cerr << "GL_ARRAY_BUFFER: " << GL_ARRAY_BUFFER << std::endl;
  std::cerr << "GL_ELEMENT_ARRAY_BUFFER: " << GL_ELEMENT_ARRAY_BUFFER
            << std::endl;
  std::cerr << "GL_PIXEL_PACK_BUFFER: " << GL_PIXEL_PACK_BUFFER << std::endl;

  // Dupe'ing what is in TFJS for now:
  glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

  GLint clip_space_pos_loc = glGetAttribLocation(program, "clipSpacePos");

  uint32_t offset = 0;
  glVertexAttribPointer(clip_space_pos_loc, 3, GL_FLOAT, false, 20,
                        reinterpret_cast<GLvoid*>(offset));
  glEnableVertexAttribArray(clip_space_pos_loc);
  check_for_errors();

  GLint uv_loc = glGetAttribLocation(program, "uv");
  if (uv_loc != -1) {  // Compiler striped?
    std::cerr << "uv_loc: " << uv_loc << std::endl;
    offset = 12;
    glVertexAttribPointer(uv_loc, 2, GL_FLOAT, false, 20,
                          reinterpret_cast<GLvoid*>(offset));
    glEnableVertexAttribArray(uv_loc);
    check_for_errors();
  }

  GLint A_loc = glGetUniformLocation(program, "A");
  GLint B_loc = glGetUniformLocation(program, "B");

  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture, 0);
  check_for_errors();
  // TODO - check frame buffer status.

  // Setup:
  glViewport(0, 0, 1, 1);
  glScissor(0, 0, 1, 1);
  glUseProgram(program);
  glUniform1f(A_loc, 1.0);
  glUniform1f(B_loc, 1.0);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture, 0);
  check_for_errors();

  // READ PIXELS!
  void* buffer = malloc(sizeof(float) * 4);
  glReadPixels(0, 0, 1, 1, GL_RGBA, GL_FLOAT, buffer);
  check_for_errors();

  std::cerr << "item: " << static_cast<float*>(buffer)[0] << std::endl;
  std::cerr << "item: " << static_cast<float*>(buffer)[1] << std::endl;
  std::cerr << "item: " << static_cast<float*>(buffer)[2] << std::endl;
  std::cerr << "item: " << static_cast<float*>(buffer)[3] << std::endl;

  //
  // TODO(kreger): Left off right here.
  //

  return 0;
}
