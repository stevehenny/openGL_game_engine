#include "MeshPlane.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderPrograms.h"
#include "Sphere.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <memory>
using glm::vec3;
class MeshPlane::Impl {
public:
  Impl()
      : shader(Shader{ShaderProgram{compiled_shaders::PLANE_GRAVITY_VERT,
                                    ShaderTypes::VERTEX},
                      ShaderProgram{compiled_shaders::PLANE_GRAVITY_FRAG,
                                    ShaderTypes::FRAGMENT}}) {
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeTriEBO);
    glGenBuffers(1, &planeLineEBO);
  }
  ~Impl() {
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &planeTriEBO);
    glDeleteBuffers(1, &planeLineEBO);
  }

  GLuint planeVAO, planeVBO, planeTriEBO, planeLineEBO;
  Shader shader;
};

MeshPlane::MeshPlane(double width, double depth, unsigned int xSegments,
                     unsigned int zSegments, float yPos, float damping)
    : width(width), depth(depth), xSegments(xSegments), zSegments(zSegments),
      yPos(yPos), damping(damping), resources(std::make_unique<Impl>()) {
  planeVertices = generateVertices(width, depth);
  planeTriIndices = generateTriIndices();
  planeLineIndices = generateLineIndices();
  planeConstraints = generatePlaneConstraints(planeVertices);

  // defined vertex data
  glBindVertexArray(resources->planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, resources->planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * planeVertices.size(),
               planeVertices.data(), GL_DYNAMIC_DRAW);

  // define the triangle EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources->planeTriEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(unsigned int) * planeTriIndices.size(),
               planeTriIndices.data(), GL_STATIC_DRAW);

  // define the Line EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources->planeLineEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               planeLineIndices.size() * sizeof(unsigned int),
               planeLineIndices.data(), GL_STATIC_DRAW);

  // define the vertex attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, position)));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, normal)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, texCoords)));

  // Unbind VAO
  glBindVertexArray(resources->planeVAO);
}

MeshPlane::~MeshPlane() = default;

void MeshPlane::updateNormals() {
  for (auto &v : planeVertices)
    v.normal = vec3(0.0f);

  for (size_t i = 0; i < planeTriIndices.size(); i += 3) {
    Vertex &v0 = planeVertices[planeTriIndices[i]];
    Vertex &v1 = planeVertices[planeTriIndices[i + 1]];
    Vertex &v2 = planeVertices[planeTriIndices[i + 2]];
    vec3 n = glm::normalize(
        glm::cross(v1.position - v0.position, v2.position - v0.position));
    v0.normal += n;
    v1.normal += n;
    v2.normal += n;
  }

  for (auto &v : planeVertices)
    v.normal = glm::normalize(v.normal);
}

auto MeshPlane::generateVertices(double width, double depth) -> vector<Vertex> {

  std::vector<Vertex> vertices;
  vertices.reserve((xSegments + 1) * (zSegments + 1));

  for (unsigned int i = 0; i <= zSegments; ++i) {
    float z = depth * ((float)i / zSegments - 0.5f);
    for (unsigned int j = 0; j <= xSegments; ++j) {
      float x = width * ((float)j / xSegments - 0.5f);

      Vertex v;
      v.position = vec3(x, yPos, z);
      v.normal = vec3(0.0f, 1.0f, 0.0f);
      v.texCoords = vec2((float)j / xSegments, (float)i / zSegments);
      v.prevPos = v.position;
      v.force = vec3(0.0f);
      v.invMass = 1.0f;

      if (i == 0 || i == zSegments || j == 0 || j == xSegments)
        v.invMass = 0.0f;

      vertices.push_back(v);
    }
  }
  return vertices;
}
auto MeshPlane::generateLineIndices() -> vector<unsigned int> {

  std::vector<unsigned int> indices;

  // horizontal lines
  for (unsigned int i = 0; i <= zSegments; ++i) {
    for (unsigned int j = 0; j < xSegments; ++j) {
      unsigned int start = i * (xSegments + 1) + j;
      unsigned int end = start + 1;
      indices.push_back(start);
      indices.push_back(end);
    }
  }

  // vertical lines
  for (unsigned int j = 0; j <= xSegments; ++j) {
    for (unsigned int i = 0; i < zSegments; ++i) {
      unsigned int start = i * (xSegments + 1) + j;
      unsigned int end = start + (xSegments + 1);
      indices.push_back(start);
      indices.push_back(end);
    }
  }

  return indices;
}

auto MeshPlane::generateTriIndices() -> vector<unsigned int> {

  std::vector<unsigned int> indices;
  indices.reserve(xSegments * zSegments * 6);

  for (unsigned int i = 0; i < zSegments; ++i) {
    for (unsigned int j = 0; j < xSegments; ++j) {
      unsigned int topLeft = i * (xSegments + 1) + j;
      unsigned int topRight = topLeft + 1;
      unsigned int bottomLeft = (i + 1) * (xSegments + 1) + j;
      unsigned int bottomRight = bottomLeft + 1;

      // two triangles per quad
      indices.push_back(topLeft);
      indices.push_back(bottomLeft);
      indices.push_back(topRight);

      indices.push_back(topRight);
      indices.push_back(bottomLeft);
      indices.push_back(bottomRight);
    }
  }
  return indices;
}

auto MeshPlane::generatePlaneConstraints(vector<Vertex> &planeVertices)
    -> vector<PlaneConstraint> {

  std::vector<PlaneConstraint> constraints;

  for (unsigned int i = 0; i <= zSegments; ++i) {
    for (unsigned int j = 0; j <= xSegments; ++j) {
      unsigned int idx = i * (xSegments + 1) + j;

      if (j < xSegments) {
        unsigned int right = idx + 1;
        float restLength = glm::length(planeVertices[idx].position -
                                       planeVertices[right].position);
        constraints.push_back({idx, right, restLength});
      }

      if (i < zSegments) {
        unsigned int down = (i + 1) * (xSegments + 1) + j;
        float restLength = glm::length(planeVertices[idx].position -
                                       planeVertices[down].position);
        constraints.push_back({idx, down, restLength});
      }

      // keep diagonals if you want shear constraints for physics
      if (i < zSegments && j < xSegments) {
        unsigned int downRight = (i + 1) * (xSegments + 1) + j + 1;
        float restLength = glm::length(planeVertices[idx].position -
                                       planeVertices[downRight].position);
        constraints.push_back({idx, downRight, restLength});
      }

      if (i < zSegments && j > 0) {
        unsigned int downLeft = (i + 1) * (xSegments + 1) + j - 1;
        float restLength = glm::length(planeVertices[idx].position -
                                       planeVertices[downLeft].position);
        constraints.push_back({idx, downLeft, restLength});
      }
    }
  }

  return constraints;
}

void MeshPlane::integrateGravity(float frameFreq) {

  for (auto &v : planeVertices) {
    if (v.invMass == 0.0f)
      continue;
    vec3 temp = v.position;
    vec3 velocity = (v.position - v.prevPos) * damping;
    v.position += velocity + v.force * frameFreq * frameFreq;

    // Simple floor constraint
    if (v.position.y < -0.0f) {
      v.position.y = -0.0f;
      v.prevPos = v.position; // stop velocity
    }

    v.prevPos = temp;
    v.force = vec3(0.0f);
  }
}
void MeshPlane::satisfyConstraints() {
  constexpr int iterations = 3;
  for (int iter = 0; iter < iterations; ++iter) {
    for (const auto &c : planeConstraints) {
      Vertex &v1 = planeVertices[c.i1];
      Vertex &v2 = planeVertices[c.i2];

      if (v1.invMass == 0.0f && v2.invMass == 0.0f)
        continue;

      glm::vec3 delta = v2.position - v1.position;
      float dist = glm::length(delta);
      if (dist == 0.0f)
        continue;
      float diff = (dist - c.restLength) / dist;

      // Position correction (split according to inverse mass)
      glm::vec3 correction = delta * 0.5f * diff;
      if (v1.invMass > 0.0f)
        v1.position += correction;
      if (v2.invMass > 0.0f)
        v2.position -= correction;
    }
  }
}
void MeshPlane::update() {

  integrateGravity(1.0f / 60.0f);
  satisfyConstraints();
  updateNormals();
}
void MeshPlane::draw() {

  glBindVertexArray(resources->planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, resources->planeVBO);

  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * planeVertices.size(),
                  planeVertices.data());

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources->planeLineEBO);
  glDrawElements(GL_LINES, static_cast<GLsizei>(planeLineIndices.size()),
                 GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void MeshPlane::applyGravity(vec3 objPos, double mass) {

  constexpr double G = 6.674e-11;
  for (auto &v : planeVertices) {
    if (v.invMass == 0.0f)
      continue;
    vec3 dir = objPos - v.position;
    float dist2 = glm::dot(dir, dir) + 0.001f; // avoid singularity
    float forceMag = G * mass / dist2;
    v.force += glm::normalize(dir) * forceMag;
  }
}
vec3 MeshPlane::getLocation() const { return vec3(0.0f, yPos, 0.0f); }
Shader &MeshPlane::getShader() const { return resources->shader; }
