#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vec3.hpp>
#include <mat4.hpp>

struct camera {
  static constexpr float YAW         = -90.0f;
  static constexpr float PITCH       =  0.0f;
  static constexpr float SPEED       =  2.5f;
  static constexpr float SENSITIVITY =  0.1f;
  static constexpr float ZOOM        =  45.0f;

  vec3 _position;
  vec3 _front;
  vec3 _up;
  vec3 _right;
  vec3 _worldUp;

  float _yaw;
  float _pitch;

  float _movementSpeed;
  float _mouseSensitivity;
  float _zoom;

  camera(vec3 position, vec3 up, float yaw, float pitch);

  mat4 getView() const;
  void updateCameraVectors();
};

#endif // CAMERA_HPP
