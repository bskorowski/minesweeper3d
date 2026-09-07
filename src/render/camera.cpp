#include "camera.hpp"

void Camera::move(v3f direction, float distance) {
  DEBUG_ASSERT(direction.x() >= -1 && direction.x() <= 1,
               std::format("X component should be normalized, but is: {}",
                           direction.x()));
  DEBUG_ASSERT(direction.y() >= -1 && direction.y() <= 1,
               std::format("Y component should be normalized, but is: {}",
                           direction.y()));
  DEBUG_ASSERT(direction.z() >= -1 && direction.z() <= 1,
               std::format("Z component should be normalized, but is: {}",
                           direction.z()));

  float leftRightDelta = direction.x();
  float upDownDelta = direction.y();
  float forwardBackwardDelta = direction.z();

  position = position + right * leftRightDelta * distance;
  position = position + this->arbitraryUp * upDownDelta * distance;
  position =
      position + this->reverseDirection * forwardBackwardDelta * distance;
}

void Camera::rotate(v3f rotations) {
  constexpr auto MAX_PITCH = 89.9F;
  constexpr auto MIN_PITCH = -89.9F;

  pitch = std::min(std::max(MIN_PITCH, pitch + rotations.x()), MAX_PITCH);
  yaw += fmod(rotations.y(), 360.0f);
  roll += fmod(rotations.z(), 360.0f);

  updateDirection();
}

void Camera::updateDirection() {
  reverseDirection.x() = cos(radians(yaw)) * cos(radians(pitch));
  reverseDirection.y() = sin(radians(pitch));
  reverseDirection.z() = sin(radians(yaw)) * cos(radians(pitch));

  reverseDirection = normalize(reverseDirection);
  right = normalize(cross(reverseDirection, arbitraryUp));
  up = normalize(cross(right, reverseDirection));
}
