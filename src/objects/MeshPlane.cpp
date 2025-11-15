#include "MeshPlane.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderPrograms.h"
#include "Spheres.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <memory>
using glm::vec3;
class MeshPlane::Impl {
public:
  Impl()
      : shader(Shader{ShaderProgram{compiled_shaders::MULTI_OBJ_PLANE_VERT,
                                    ShaderTypes::VERTEX},
                      ShaderProgram{compiled_shaders::MULTI_OBJ_PLANE_FRAG,
                                    ShaderTypes::FRAGMENT}}) {
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeTriEBO);
    glGenBuffers(1, &planeLineEBO);
    glGenBuffers(1, &SBBO);
  }
  ~Impl() {
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &planeTriEBO);
    glDeleteBuffers(1, &planeLineEBO);
    glDeleteBuffers(1, &SBBO);
  }

  GLuint planeVAO, planeVBO, planeTriEBO, planeLineEBO, SBBO;
  Shader shader;
};

MeshPlane::MeshPlane(double width, double depth, unsigned int xSegments,
                     unsigned int zSegments, float yPos, float damping)
    : Object(), width(width), depth(depth), xSegments(xSegments),
      zSegments(zSegments), yPos(yPos), damping(damping),
      resources(std::make_unique<Impl>()) {
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

void MeshPlane::setSphereSBBO(GLuint SpheresSBBO, size_t SizeOfSBBO) {
  this->spheresSBBO = SpheresSBBO;
  this->SizeOfSBBO = SizeOfSBBO;
}

void MeshPlane::updateSBBO_from_GPU() const {

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, spheresSBBO);
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

void MeshPlane::draw() {

  glBindVertexArray(resources->planeVAO);
  // glBindBuffer(GL_ARRAY_BUFFER, resources->planeVBO);

  // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * planeVertices.size(),
  //                 planeVertices.data());

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources->planeLineEBO);
  glDrawElements(GL_LINES, static_cast<GLsizei>(planeLineIndices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, spheresSBBO);
  glBindVertexArray(0);
}

vec3 MeshPlane::getLocation() const { return vec3(0.0f, yPos, 0.0f); }
Shader &MeshPlane::getShader() const { return resources->shader; }
void MeshPlane::updateSBBO(vector<Sphere> &objects) const {

  // define SBBO
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, resources->SBBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Sphere) * objects.size(),
               objects.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, resources->SBBO);
}
