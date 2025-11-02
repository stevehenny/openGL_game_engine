#define STB_TRUETYPE_IMPLEMENTATION
#include "TextRender.h"
#include "external/stb/stb_truetype.h"
#include <fstream>
#include <iostream>
#include <vector>

TextRender::TextRender(const std::string &fontFile, GLuint shader,
                       int screenWidth, int screenHeight, float fontSize)
    : shaderProgram(shader) {
  projection = glm::ortho(0.0f, float(screenWidth), 0.0f, float(screenHeight));

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr,
               GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Load font
  std::ifstream file(fontFile, std::ios::binary | std::ios::ate);
  if (!file) {
    std::cerr << "Failed to load font: " << fontFile << "\n";
    return;
  }
  size_t size = file.tellg();
  file.seekg(0);
  std::vector<unsigned char> buffer(size);
  file.read((char *)buffer.data(), size);

  stbtt_fontinfo font;
  if (!stbtt_InitFont(&font, buffer.data(), 0)) {
    std::cerr << "Failed to initialize font\n";
    return;
  }

  float scale = stbtt_ScaleForPixelHeight(&font, fontSize);

  for (unsigned char c = 32; c < 128; ++c) {
    int w, h, xoff, yoff;
    unsigned char *bitmap =
        stbtt_GetCodepointBitmap(&font, 0, scale, c, &w, &h, &xoff, &yoff);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE,
                 bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int advance, lsb;
    stbtt_GetCodepointHMetrics(&font, c, &advance, &lsb);

    Character character = {tex, glm::ivec2(w, h), glm::ivec2(xoff, yoff),
                           GLuint(advance * scale)};
    Characters.insert(std::pair<char, Character>(c, character));

    stbtt_FreeBitmap(bitmap, nullptr);
  }
}

void TextRender::RenderText(const std::string &text, float x, float y,
                            float scale, glm::vec3 color) {
  glUseProgram(shaderProgram);
  glUniform3f(glGetUniformLocation(shaderProgram, "textColor"), color.x,
              color.y, color.z);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
                     GL_FALSE, &projection[0][0]);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  for (auto c : text) {
    Character ch = Characters[c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    float vertices[6][4] = {
        {xpos, ypos + h, 0.0f, 0.0f},    {xpos, ypos, 0.0f, 1.0f},
        {xpos + w, ypos, 1.0f, 1.0f},

        {xpos, ypos + h, 0.0f, 0.0f},    {xpos + w, ypos, 1.0f, 1.0f},
        {xpos + w, ypos + h, 1.0f, 0.0f}};

    glBindTexture(GL_TEXTURE_2D, ch.TextureID);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    x += (ch.Advance) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
