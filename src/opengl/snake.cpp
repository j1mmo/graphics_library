#include "include/snake.hpp"

vec2 snake::direction_to_coord(snake::Direction direction)
{
  if (direction == Direction::UP) {
      return vec2{0, -1};
  } else if (direction == Direction::DOWN) {
      return vec2{0, 1};
  } else if (direction == Direction::LEFT) {
      return vec2{-1, 0};
  } else if (direction == Direction::RIGHT) {
      return vec2{1, 0};
  } else {
      return vec2{0,0}; //this shouldn't hit
  }
}
