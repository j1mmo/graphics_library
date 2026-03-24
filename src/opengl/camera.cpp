#include <camera.hpp>

#include <general.hpp>

#include <cmath>

camera::camera(vec3 position = vec3(0.0f, 0.0f, 0.0f),
	       vec3 up = vec3(0.0f, 1.0f, 0.0f),
	       float yaw = camera::YAW,
	       float pitch = camera::PITCH)
  : _front(vec3(0.0f, 0.0f, -1.0f)),
    _movementSpeed(camera::SPEED),
    _mouseSensitivity(camera::SENSITIVITY),
    _zoom(camera::ZOOM)
{
  _position = position;
  _worldUp = up;
  _yaw = yaw;
  _pitch = pitch;
  updateCameraVectors();
}

mat4 camera::getView() const
{
  return mat4::lookAt(_position, _position + _front, _up);
}

void camera::updateCameraVectors()
{
  // calculate the new Front vector
  vec3 front;
  front[0] = cos(maths::radians(_yaw)) * cos(maths::radians(_pitch));
  front[1] = sin(maths::radians(_pitch));
  front[2] = sin(maths::radians(_yaw)) * cos(maths::radians(_pitch));
  _front = maths::normalise(_front);
  // also re-calculate the Right and Up vector
  _right = maths::normalise(maths::cross(_front, _worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  _up    = maths::normalise(maths::cross(_right, _front));
}
