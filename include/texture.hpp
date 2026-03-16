#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "types.hpp"

struct Texture {
  u32 handle;

  void bind() const;
  void release();
};

namespace texture {
  Texture load(const char* imageLocation);
}

#endif //TEXTURE_HPP
