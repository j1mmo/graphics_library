#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vec3.hpp>
#include <mat4.hpp>

struct Camera {
  static constexpr float YAW         = -90.0f;
  static constexpr float PITCH       =  -80.0f;
  static constexpr float SPEED       =  2.5f;
  static constexpr float SENSITIVITY =  0.1f;
  static constexpr float ZOOM        =  45.0f;

  enum movement {
    forward,
    backwards,
    left,
    right
  };

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

  Camera(vec3 position = vec3(3.2863748, 12.975187, 4.943796), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = Camera::YAW, float pitch = Camera::PITCH);
  void processKeyboard(movement direction, float deltaTime);
  void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
  void changeFov(float offset);
  mat4 getView() const;
  void updateCameraVectors();
};

#endif // CAMERA_HPP
