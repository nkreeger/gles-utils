precision highp float;
precision highp int;
varying vec2 resultUV;
const vec2 halfCR = vec2(0.5, 0.5);

struct ivec5
{
  int x;
  int y;
  int z;
  int w;
  int u;
};

struct ivec6
{
  int x;
  int y;
  int z;
  int w;
  int u;
  int v;
};

bool isNaN(float val) {
  return (val < 1.0 || 0.0 < val || val == 0.0) ? false : true;
}

bool hasNaN(vec4 values) {
  vec4 v1 = values * values;
  vec4 v2 = values * values;
  return any(notEqual(v1, v2));
}

float getNaN(vec4 values) {
  return dot(vec4(1), values);
}

int round(float value) {
  return int(floor(value + 0.5));
}

int imod(int x, int y) {
  return x - y * (x / y);
}

//Based on the work of Dave Hoskins
//https://www.shadertoy.com/view/4djSRW
#define HASHSCALE1 443.8975
float random(float seed){
  vec2 p = resultUV * seed;
  vec3 p3  = fract(vec3(p.xyx) * HASHSCALE1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

vec2 UVfrom1D(int texNumR, int texNumC, int index) {
  int texR = index / texNumC;
  int texC = index - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}

vec2 packedUVfrom1D(int texNumR, int texNumC, int index) {
  int texelIndex = index / 2;
  int texR = texelIndex / texNumC;
  int texC = texelIndex - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}

vec2 UVfrom2D(int texNumR, int texNumC, int numC, int row, int col) {
  int index = row * numC + col;
  int texR = index / texNumC;
  int texC = index - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}

vec2 packedUVfrom2D(int texelsInLogicalRow, int texNumR,
    int texNumC, int row, int col) {
  int texelIndex = (row / 2) * texelsInLogicalRow + (col / 2);
  int texR = texelIndex / texNumC;
  int texC = texelIndex - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}

vec2 UVfrom3D(int texNumR, int texNumC, int stride0,
    int stride1, int row, int col, int depth) {
  // Explicitly use integer operations as dot() only works on floats.
  int index = row * stride0 + col * stride1 + depth;
  int texR = index / texNumC;
  int texC = index - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}

vec2 UVfrom4D(int texNumR, int texNumC, int stride0,
    int stride1, int stride2, int row, int col, int depth,
    int depth2) {
  // Explicitly use integer operations as dot() only works on floats.
  int index = row * stride0 + col * stride1 + depth * stride2 + depth2;
  int texR = index / texNumC;
  int texC = index - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}

vec2 packedUVfrom4D(int texNumR, int texNumC, int texelsInBatch2,
    int texelsInBatch, int texelsInLogicalRow, int b2, int b,
    int row, int col) {
  int index = b2 * texelsInBatch2 + b * texelsInBatch +
    (row / 2) * texelsInLogicalRow + (col / 2);
  int texR = index / texNumC;
  int texC = index - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}

vec2 UVfrom5D(int texNumR, int texNumC, int stride0,
    int stride1, int stride2, int stride3, int row, int col, int depth,
    int depth2, int depth3) {
  // Explicitly use integer operations as dot() only works on floats.
  int index = row * stride0 + col * stride1 +
    depth * stride2 + depth2 * stride3 + depth3;
  int texR = index / texNumC;
  int texC = index - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}


vec2 UVfrom6D(int texNumR, int texNumC, int stride0,
    int stride1, int stride2, int stride3, int stride4,
    int row, int col, int depth, int depth2, int depth3, int depth4) {
  // Explicitly use integer operations as dot() only works on floats.
  int index = row * stride0 + col * stride1 + depth * stride2 + depth2 *
    stride3 + depth3 * stride4 + depth4;
  int texR = index / texNumC;
  int texC = index - texR * texNumC;
  return (vec2(texC, texR) + halfCR) / vec2(texNumC, texNumR);
}

float sampleTexture(sampler2D textureSampler, vec2 uv) {
  return texture2D(textureSampler, uv).r;
}

void setOutput(float val) {
  gl_FragColor = vec4(val, 0, 0, 0);
}


uniform float A;
uniform float B;

int getOutputCoords() {
  return 0;
}

float getAFlat(int index) {return A;}float getA() {return A;}float getAAtOutCoords() {return A;}
float getBFlat(int index) {return B;}float getB() {return B;}float getBAtOutCoords() {return B;}

uniform float NAN;
float binaryOperation(float a, float b) {
  return a + b;
}

void main() {
  float a = getAAtOutCoords();
  float b = getBAtOutCoords();
  setOutput(binaryOperation(a, b));
}
