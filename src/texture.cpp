#include "texture.hpp"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "stb_image.h"

void Texture::release() {
  glDeleteTextures(1, &handle);
}

void Texture::bind() const {
  glBindTexture(GL_TEXTURE_2D, handle);
}

Texture texture::load(const char* imageLocation)
{
  u32 textureHandle{0};

  glGenTextures(1, &textureHandle);
  glBindTexture(GL_TEXTURE_2D, textureHandle);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  i32 width{0}, height{0}, nrChannels{0};
  // 3 - forces RGB format
  uchar* data = stbi_load(imageLocation, &width, &height, &nrChannels, 3);

  if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      stbi_image_free(data);
  }
  else {
      spdlog::error("unable to load texture");
  }

  return Texture{textureHandle};
}
