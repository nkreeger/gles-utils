#ifndef GLES_UTILS_H_
#define GLES_UTILS_H_

#include <fstream>
#include <iostream>
#include <string>

// #include <GLES3/gl32.h>

//
// Checks for any pending GL errors and logs to std::cerr.
//
bool check_for_errors() {
  GLenum error;
  bool had_error = false;
  while ((error = glGetError()) != GL_NO_ERROR) {
    std::cerr << "HAS ERRORS -----" << std::endl;
    switch (error) {
      case GL_INVALID_ENUM:
        std::cerr << "Found unchecked GL error: GL_INVALID_ENUM" << std::endl;
        break;
      case GL_INVALID_VALUE:
        std::cerr << "Found unchecked GL error: GL_INVALID_VALUE" << std::endl;
        break;
      case GL_INVALID_OPERATION:
        std::cerr << "Found unchecked GL error: GL_INVALID_OPERATION"
                  << std::endl;
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cerr
            << "Found unchecked GL error: GL_INVALID_FRAMEBUFFER_OPERATION"
            << std::endl;
        break;
      case GL_OUT_OF_MEMORY:
        std::cerr << "Found unchecked GL error: GL_OUT_OF_MEMORY" << std::endl;
        break;
      default:
        std::cerr << "Found unchecked GL error: UNKNOWN ERROR" << std::endl;
        break;
    }
  }
  return had_error;
}

//
// Sets shader source from a given file name.
//
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

//
// Creates a texture with given internalFormat, format, and type.
//
void create_texture_2d(GLint internal_format, GLenum format, GLenum type,
                       GLuint* texture) {
  std::cerr << "------- CREATE TEXTURE -----------------" << std::endl;
  std::cerr << "* internalFormat: 0x" << std::hex << internal_format
            << std::endl;
  std::cerr << "* format        : 0x" << std::hex << format << std::endl;
  std::cerr << "* type          : 0x" << std::hex << type << std::endl;
  std::cerr << "----------------------------------------" << std::endl;

  glGenTextures(1, texture);
  glBindTexture(GL_TEXTURE_2D, *texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, 1, 1, 0, format, type,
               nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);
}

//
// Bootstraps environment for offscreen GL math.
//
void bootstrap_env(GLuint* vertex_buffer, GLuint* index_buffer,
                   GLuint* framebuffer) {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_DITHER);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glDisable(GL_SAMPLE_COVERAGE);
  glEnable(GL_SCISSOR_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  if (vertex_buffer) {
    glGenBuffers(1, vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer);
    float coords[] = {-1.f, 1.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f, 0.f,
                      1.f,  1.f, 0.f, 1.f, 1.f, 1.f,  -1.f, 0.f, 1.f, 0.f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
  }

  if (index_buffer) {
    glGenFramebuffers(1, index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer);
    uint16_t indices[] = {0, 1, 2, 2, 1, 3};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
  }

  if (framebuffer) {
    glGenFramebuffers(1, framebuffer);
  }
}

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

#endif  // GLES_UTILS_H_
