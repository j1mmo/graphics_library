#include <camera.hpp>

#include <general.hpp>
#include <cmath>

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch)
  : _front(vec3(0.0f, 0.0f, -1.0f)),
    _movementSpeed(Camera::SPEED),
    _mouseSensitivity(Camera::SENSITIVITY),
    _zoom(Camera::ZOOM)
{
  _position = position;
  _worldUp = up;
  _yaw = yaw;
  _pitch = pitch;
  updateCameraVectors();
}

void Camera::processKeyboard(movement direction, float deltaTime)
{
  const float velocity = _movementSpeed * deltaTime;
  if (direction == movement::forward) {
      _position += _front * velocity;
  }
  if (direction == movement::backwards) {
      _position -= _front * velocity;
  }
  if (direction == movement::left) {
      _position -= _right * velocity;
  }
  if (direction == movement::right) {
      _position += _right * velocity;
  }

}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
  xOffset *= _mouseSensitivity;
  yOffset *= _mouseSensitivity;

  _yaw += xOffset;
  _pitch += yOffset;

  if (constrainPitch) {
      if (_pitch > 89.0f) {
	  _pitch = 89.0f;
      }
      if (_pitch < -89.0f) {
	  _pitch = -89.0f;
      }
  }
  
  updateCameraVectors();
}

void Camera::changeFov(float offset)
{
  _zoom -= (float)offset;
  if (_zoom < 1.0f) {
      _zoom = 1.0f;
  }
  if (_zoom > 45.0f) {
      _zoom = 45.0f;
  }
}

mat4 Camera::getView() const
{
  return mat4::lookAt(_position, _position + _front, _up);
}

void Camera::updateCameraVectors()
{
  // calculate the new Front vector
  vec3 front;
  front[0] = cos(maths::radians(_yaw)) * cos(maths::radians(_pitch));
  front[1] = sin(maths::radians(_pitch));
  front[2] = sin(maths::radians(_yaw)) * cos(maths::radians(_pitch));
  _front = maths::normalise(front);
  // also re-calculate the Right and Up vector
  _right = maths::normalise(maths::cross(_front, _worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  _up    = maths::normalise(maths::cross(_right, _front));
}
