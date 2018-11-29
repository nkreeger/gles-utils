precision highp float;
attribute vec3 clipSpacePos;
attribute vec2 uv;
varying vec2 resultUV;

void main() {
  gl_Position = vec4(clipSpacePos, 1);
  resultUV = uv;
}

