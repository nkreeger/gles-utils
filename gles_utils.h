#ifndef GLES_UTILS_H_
#define GLES_UTILS_H_

#include <iostream>
#include <fstream>
#include <string>

#include <GLES3/gl32.h>

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
        std::cerr << "Found unchecked GL error: GL_INVALID_OPERATION" << std::endl;
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cerr << "Found unchecked GL error: GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
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


#endif  // GLES_UTILS_H_
