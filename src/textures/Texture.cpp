#include "Texture.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
unsigned int loadTexture(const char *texturePath) {
  // Initialize the texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate the texture
  int texture_width, texture_height, nrChannels;
  unsigned char *data =
      stbi_load(texturePath, &texture_width, &texture_height, &nrChannels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
    exit(1);
  }
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}

GLuint loadTextureArray(const std::vector<const char *> texturePaths) {
  if (texturePaths.empty()) {
    std::cerr << "Error: No textures provided for Texture2DArray.\n";
    exit(1);
  }

  int width = 0, height = 0, nrChannels = 0;
  unsigned char *data =
      stbi_load(texturePaths[0], &width, &height, &nrChannels, 0);
  if (!data) {
    std::cerr << "Failed to load texture: " << texturePaths[0] << std::endl;
    exit(1);
  }
  stbi_image_free(data);

  GLuint textureArray;
  glGenTextures(1, &textureArray);
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

  GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

  // Allocate storage for the texture array
  glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1,
                 (format == GL_RGBA ? GL_RGBA8 : GL_RGB8), width, height,
                 static_cast<GLsizei>(texturePaths.size()));

  // Load each layer
  for (size_t layer = 0; layer < texturePaths.size(); ++layer) {
    int w, h, c;
    unsigned char *layerData = stbi_load(texturePaths[layer], &w, &h, &c, 0);
    if (!layerData) {
      std::cerr << "Failed to load texture: " << texturePaths[layer]
                << std::endl;
      exit(1);
    }

    if (w != width || h != height || c != nrChannels) {
      std::cerr << "Error: All textures in the array must have the same "
                   "dimensions and channels!\n";
      exit(1);
    }

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, static_cast<GLint>(layer),
                    width, height, 1, format, GL_UNSIGNED_BYTE, layerData);

    stbi_image_free(layerData);
  }

  // Set filtering/wrapping
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

  return textureArray;
}
