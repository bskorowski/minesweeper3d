#include "math/matrix.hpp"

struct Color {
  constinit inline static v3f White = vec3(1.0F, 1.0F, 1.0F);
  constinit inline static v3f Gray = vec3(0.8F, 0.85F, 0.89F);
  constinit inline static v3f Red = vec3(1.0F, 0.0F, 0.0F);
  constinit inline static v3f Purple = vec3(0.5F, 0.0F, 0.5F);
  constinit inline static v3f Orange = vec3(1.0F, 0.64F, 0.0F);
  constinit inline static v3f Yellow = vec3(1.0F, 1.0F, 0.0F);
  constinit inline static v3f Green = vec3(0.0F, 1.0F, 0.0F);
  constinit inline static v3f DarkGray = vec3(0.1F, 0.1F, 0.1F);
  constinit inline static v3f Black = vec3(0.0F, 0.0F, 0.0F);
};
