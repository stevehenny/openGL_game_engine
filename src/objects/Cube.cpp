#include "Cube.h"
#include "Shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <mutex>

using glm::mat4;
using glm::vec3;

constexpr unsigned int PROJECTION_LOC = 2;
constexpr unsigned int VIEW_LOC = 3;
constexpr unsigned int MODEL_LOC = 4;

Cube::Cube(vec3 global_position, unsigned int texture, Shader shader)
    : global_position(global_position), texture(texture), shader(shader) {
  std::lock_guard<std::mutex> lock(instance_mut);
  if (instance_count == 0) {
    init();
  }
  ++instance_count;
}

Cube::~Cube() {
  std::lock_guard<std::mutex> lock(instance_mut);
  instance_count--;
  if (instance_count == 0) {
    cleanup();
  }
}

void Cube::move(vec3 translation) { global_position += translation; }

void Cube::draw() {
  glBindVertexArray(globalVAO);

  mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, global_position);

  shader.setMat4(MODEL_LOC, model);

  glDrawArrays(GL_TRIANGLES, 0, CUBE_POINTS);
}

vec3 &Cube::get_position() { return global_position; }

void Cube::setTexture() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
}

void Cube::setModelMat4(mat4 &model) { shader.setMat4(MODEL_LOC, model); }
void Cube::setProjectionMat4(mat4 &projection) {
  shader.setMat4(PROJECTION_LOC, projection);
}
void Cube::setViewMatrix(mat4 &view) { shader.setMat4(VIEW_LOC, view); }
