#pragma once

#include "debug_utils.hpp"
#include "math/math.hpp"
#include "math/matrix.hpp"
#include <logzy/logzy.hpp>

struct Camera {

  constexpr Camera(v3f initialPosition, v3f arbitraryUp)
      : position(initialPosition), up(arbitraryUp),
        right(cross(reverseDirection, arbitraryUp)), arbitraryUp(arbitraryUp) {
    updateDirection();
  }

  // Rotation around X axis (up-down) in degrees.
  float pitch = 0.0F;
  // Rotation around Y axis (left-right) in degrees.
  float yaw = 90.0F;
  // Rotation around Z axis (rolling left-right) in degrees.
  float roll = 0.0F;
  // Camera world position
  v3f position = vec3<float>(0.0F, 0.0F, 0.0F);
  // Reverse of the direction the camera is looking at
  v3f reverseDirection = vec3<float>(0.0F, 0.0F, -1.0F);
  // camera up vector
  v3f up = vec3<float>(0.0F, 1.0F, 0.0F);
  // camera up vector
  v3f right = vec3<float>(1.0F, 0.0F, 0.0F);
  // World up vector
  v3f arbitraryUp = vec3<float>(0.0F, 1.0F, 0.0F);

  [[nodiscard]] constexpr m4x4f getView() {
    return lookAt(position, reverseDirection, up, right);
  }

  constexpr v3f getDirection() { return reverseDirection * -1; }

  /**
   * Moves the camera by the given delta position
   * X component = left-right
   * Y component = up-down
   * Z component = forward-backward
   */
  void move(v3f direction, float distance);

  /**
   * Rotates the camera along each axis by given degrees.
   */
  void rotate(v3f rotations);

private:
  void updateDirection();
};
