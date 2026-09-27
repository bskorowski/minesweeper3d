#include "camera.hpp"

void Camera::move(v3f direction, float distance) {
  ASSERT(direction.x() >= -1 && direction.x() <= 1,
         "X component should be normalized, but is: {}", direction.x());
  ASSERT(direction.y() >= -1 && direction.y() <= 1,
         "Y component should be normalized, but is: {}", direction.y());
  ASSERT(direction.z() >= -1 && direction.z() <= 1,
         "Z component should be normalized, but is: {}", direction.z());

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
