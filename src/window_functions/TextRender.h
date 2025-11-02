#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <string>

struct Character {
  GLuint TextureID;   // Glyph texture ID
  glm::ivec2 Size;    // Glyph size in pixels
  glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
  GLuint Advance;     // Offset to advance to next glyph
};

class TextRender {
public:
  std::map<char, Character> Characters;
  GLuint VAO, VBO;
  GLuint shaderProgram;
  glm::mat4 projection;

  TextRender(const std::string &fontFile, GLuint shader, int screenWidth,
             int screenHeight, float fontSize);

  void RenderText(const std::string &text, float x, float y, float scale,
                  glm::vec3 color);
};
