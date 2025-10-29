#pragma once
#include "Shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <mutex>

using glm::vec3, glm::mat4, std::cout;
class Cube {
public:
  Cube(vec3 global_position, unsigned int texture, Shader shader);
  ~Cube();
  void move(vec3 translation);
  // void kill();
  void draw();
  vec3 &get_position();
  void setTexture();
  void setModelMat4(mat4 &model);
  void setProjectionMat4(mat4 &projection);
  void setViewMatrix(mat4 &view);

private:
  static inline unsigned int globalVAO = 0;
  static inline unsigned int globalVBO = 0;
  static inline unsigned int globalEBO = 0;
  static inline unsigned int instance_count = 0;
  vec3 global_position;
  unsigned int texture;
  Shader shader;
  static constexpr unsigned int CUBE_POINTS = 36;
  std::mutex instance_mut;

  // single init for all cube objects
  static void init() {
    glGenVertexArrays(1, &globalVAO);
    glGenBuffers(1, &globalVBO);
    glGenBuffers(1, &globalEBO);

    glBindVertexArray(globalVAO);

    glBindBuffer(GL_ARRAY_BUFFER, globalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, globalEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // define attribute pointers
    // global position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    // texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // successfully init
    cout << "Successfully allocated buffers\n";
  }

  static void cleanup() {
    cout << "cleaning up Cube Buffers\n";
    glDeleteVertexArrays(1, &globalVAO);
    glDeleteBuffers(1, &globalVBO);
    glDeleteBuffers(1, &globalEBO);
    globalVAO = globalVBO = globalEBO = 0;
  }

  static constexpr float vertices[180] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
  static constexpr unsigned int indices[6] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };
};
