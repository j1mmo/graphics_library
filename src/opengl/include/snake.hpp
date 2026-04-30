#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "types.hpp"

#include <array.hpp>
#include <vec2.hpp>

#include <cstdlib>
#include <cstdio>

namespace snake
{
  
  struct Grid {
    enum class Types : u8 {
      EMPTY = 0,
      BODY = 1,
      FOOD = 2,
    };
    constexpr static u32 WIDTH = 8;
    constexpr static u32 HEIGHT = 8;
    constexpr static u32 SIZE = WIDTH * HEIGHT;
    array<Types, SIZE> data;
  };

  enum class Direction : u8 {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STILL,
  };

  vec2 direction_to_coord(Direction direction);

  struct Player
  {
    vec2 head{0,0};
    vec2 previous_position_{0,0};
    array<vec2, Grid::SIZE> body;
    u32 body_length_{0};
    Direction direction{snake::Direction::STILL};

    void move_body() {
      for (int i = body_length_ - 1; i > 0; --i) {
	  body[i] = body[i - 1];
      }
      body[0] = head;
    }
  };

  struct game_state {
    Player player_;
    vec2 food_{2,2};

    bool check_collision() {
      int px = (int)player_.head[0];
      int py = (int)player_.head[1];
      
      int fx = (int)food_[0];
      int fy = (int)food_[1]; 

      if (px == fx && py == fy) {
	  return true;
      }
      
      return false;
    }

    bool loop() {
      bool growing = check_collision();
      

      if (!growing) {
	  //todo
      } else {
	  player_.body_length_++;
	  
	  array<Grid::Types, Grid::SIZE> grid;
	  for (Grid::Types& t : grid) {
	      t = Grid::Types::EMPTY;
	  }

	  for(int i = 0; i < player_.body_length_; i++) {
	      vec2 body_part = player_.body[i];
	      if (body_part[0] >= 0 && body_part[1] >= 0) {
		  grid[body_part[0] + body_part[1] * Grid::WIDTH] = Grid::Types::BODY;
	      }
	  }
	  array<vec2, Grid::SIZE> free_spaces;
	  int free_spaces_size{0};
	  for(int x = 0; x < Grid::WIDTH; x++) {
	      for(int y = 0; y < Grid::HEIGHT; y++) {
		  if (grid[x + y * Grid::WIDTH] == Grid::Types::EMPTY) {
		      free_spaces[free_spaces_size] = vec2{(float)x, (float)y};
		      free_spaces_size += 1;
		  }
	      }
	  }
	  vec2 new_food_location = free_spaces[rand() % free_spaces_size];
	  food_ = new_food_location;
      }

      player_.move_body();
      player_.head += direction_to_coord(player_.direction);

      return growing;
    }
  };
}


#endif // SNAKE_HPP
