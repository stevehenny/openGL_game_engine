#include "Spheres.h"
#include "ShaderProgram.h"
#include "ShaderPrograms.h"
#include "top_level_include.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using glm::vec3, glm::vec2;

///*************SPHERE CLASS**************
class Spheres::Impl {
public:
  Impl()
      : shader(Shader{ShaderProgram{compiled_shaders::SPHERES_GPU_VERT,
                                    ShaderTypes::VERTEX},
                      ShaderProgram{compiled_shaders::SPHERES_GPU_FRAG,
                                    ShaderTypes::FRAGMENT}}),
        computeShader(Shader{ShaderProgram{compiled_shaders::GRAVITY_COMP,
                                           ShaderTypes::COMPUTE}}) {
    glGenVertexArrays(1, &SpheresVAO);
    glGenBuffers(1, &SpheresVBO);
    glGenBuffers(1, &SpheresEBO);
    glGenBuffers(1, &SpheresSBBO);
    glGenBuffers(1, &SpheresTexturesSBBO);
  }

  ~Impl() {
    glDeleteVertexArrays(1, &SpheresVAO);
    glDeleteBuffers(1, &SpheresVBO);
    glDeleteBuffers(1, &SpheresEBO);
    glDeleteBuffers(1, &SpheresSBBO);
    glDeleteBuffers(1, &SpheresTexturesSBBO);
  }

  // Sphere needs the following:
  // dataO: For attribute pointers
  // VBO: For VertexArray data
  // EBO: For indices -> Tell the GPU what order to draw
  // shader program: GPU code to draw the sphere
  GLuint SpheresVAO, SpheresVBO, SpheresEBO, SpheresSBBO, SpheresTexturesSBBO;
  Shader shader;
  Shader computeShader;
};

Spheres::Spheres(float radius, size_t sectorCount, size_t stackCount)
    : radius(radius), stackCount(stackCount), sectorCount(sectorCount),
      resources(std::make_unique<Impl>()) {

  // Generate VertexArray and indices for EBO
  data = this->generateVertexArray();
  indices = this->generateIndices();

  // implementation of allocating buffers and
  // setting dataO attribute pointers

  //
  glBindVertexArray(resources->SpheresVAO);

  // upload vertex data
  size_t vertexCount = (sectorCount + 1) * (stackCount + 1);
  size_t totalBytes =
      (reinterpret_cast<uintptr_t>(data.texCoords + vertexCount) -
       reinterpret_cast<uintptr_t>(data.begin));

  glBindBuffer(GL_ARRAY_BUFFER, resources->SpheresVBO);
  glBufferData(GL_ARRAY_BUFFER, totalBytes, data.begin, GL_STATIC_DRAW);

  // upload index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources->SpheresEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  // upload seperate SBBO buffer of seperate
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, resources->SpheresSBBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere),
               spheres.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, resources->SpheresSBBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  // attribute pointers
  auto base = reinterpret_cast<uintptr_t>(data.begin);
  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, 0,
      (void *)(reinterpret_cast<uintptr_t>(data.position) - base));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, 0,
      (void *)(reinterpret_cast<uintptr_t>(data.normal) - base));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE, 0,
      (void *)(reinterpret_cast<uintptr_t>(data.texCoords) - base));
  glEnableVertexAttribArray(2);
}

Spheres::~Spheres() = default;

void Spheres::renderSphere() {}
void Spheres::draw() {
  glBindVertexArray(resources->SpheresVAO);
  glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                          GL_UNSIGNED_INT, 0,
                          static_cast<GLsizei>(spheres.size()));
}

Shader &Spheres::getComputeShader() { return resources->computeShader; }
Shader &Spheres::getShader() { return resources->shader; }
vector<Sphere> &Spheres::getSpheres() { return spheres; }
VertexArray &Spheres::getVertexArray() { return data; }
vector<GLuint> &Spheres::getIndices() { return indices; }
void Spheres::addSphere(const Sphere &sphere) {
  spheres.push_back(sphere);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, resources->SpheresSBBO);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, spheres.size() * sizeof(Sphere),
                  spheres.data());
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, resources->SpheresSBBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

GLuint Spheres::getSphereSBBO() { return resources->SpheresSBBO; }

void Spheres::runComputeShader(float dt) {

  resources->computeShader.useShader();
  resources->computeShader.setFloat(
      glGetUniformLocation(resources->computeShader.ID, "dt"), dt);
  resources->computeShader.setFloat(
      glGetUniformLocation(resources->computeShader.ID, "G"),
      physics_constants::G);
  resources->computeShader.setUInt(
      glGetUniformLocation(resources->computeShader.ID, "numSpheres"),
      spheres.size());

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, resources->SpheresSBBO);
  GLuint groups = (spheres.size() + 255) / 256;
  glDispatchCompute(groups, 1, 1);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Spheres::bindComputeToSBBO() const {}

void Spheres::computeForces() {
  for (auto &body : spheres) {
    // initialize vector to zero vec4
    body.force = vec4(0.0f);

    for (size_t i = 0; i < spheres.size(); ++i) {
      for (size_t j = i + 1; j < spheres.size(); ++j) {
        vec3 r_vec = vec3(spheres[j].position - spheres[i].position);
        float r_mag = glm::length(r_vec);
        if (r_mag == 0)
          continue;

        vec3 force_dir = r_vec / r_mag;
        float force_mag = physics_constants::G * spheres[i].mass *
                          spheres[j].mass / (r_mag * r_mag);
        vec3 f_vec = force_mag * force_dir;

        spheres[i].force += vec4(f_vec, 0.0);
        spheres[j].force -= vec4(f_vec, 0.0);
      }
    }
  }
}

void Spheres::applyGravity(float dt) {
  computeForces();

  for (auto &body : spheres) {
    glm::vec3 acceleration = glm::vec3(body.force) / body.mass;
    // Half-step velocity update
    body.velocity += glm::vec4(acceleration * dt * 0.5f, 0.0);
    body.position += glm::vec4(glm::vec3(body.velocity) * dt, 0.0);
  }
  // recompute after moving
  computeForces();

  for (auto &body : spheres) {
    glm::vec3 acceleration = glm::vec3(body.force) / body.mass;
    // Second half-step velocity update
    body.velocity += glm::vec4(acceleration * dt * 0.5f, 0.0);
  }
}

double Spheres::computeTotalEnergy() {
  double kinetic = 0.0;
  double potential = 0.0;

  for (size_t i = 0; i < spheres.size(); ++i) {
    const auto &s_i = spheres[i];
    glm::dvec3 v = glm::dvec3(s_i.velocity);
    kinetic += 0.5 * s_i.mass * glm::dot(v, v);

    for (size_t j = i + 1; j < spheres.size(); ++j) {
      glm::dvec3 r = glm::dvec3(spheres[j].position - s_i.position);
      double dist = glm::length(r);
      dist = std::max(dist, 1e-6); // softening to avoid singularities
      potential -= physics_constants::G * s_i.mass * spheres[j].mass / dist;
    }
  }

  return kinetic + potential;
}
void Spheres::updateSBBO(vector<Sphere> &newSpheres) {
  spheres = newSpheres;
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, resources->SpheresSBBO);

  glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere),
               spheres.data(),
               GL_DYNAMIC_DRAW); // <-- use glBufferData, not SubData

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, resources->SpheresSBBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Spheres::updateSBBO() {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, resources->SpheresSBBO);

  glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
                     spheres.size() * sizeof(Sphere), spheres.data());
  // glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere),
  //              spheres.data(), GL_DYNAMIC_DRAW);
  //
  // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, resources->SpheresSBBO);
  // glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Spheres::updateTextures(std::vector<GLuint64> &handles) {
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, resources->SpheresTexturesSBBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, handles.size() * sizeof(GLuint64),
               handles.data(), GL_STATIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 20,
                   resources->SpheresTexturesSBBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

auto Spheres::generateVertexArray() -> VertexArray {

  // Number of vertices
  size_t vertexCount = (stackCount + 1) * (sectorCount + 1);
  VertexArray vertexArray;
  vertexArray.resize(vertexCount);

  // --- Fill vertex data ---
  size_t index = 0;
  for (unsigned int i = 0; i <= stackCount; ++i) {
    float stackAngle =
        glm::pi<float>() / 2 - (float)i * glm::pi<float>() / stackCount;
    float xy = radius * cosf(stackAngle);
    float z = radius * sinf(stackAngle);

    for (unsigned int j = 0; j <= sectorCount; ++j, ++index) {
      float sectorAngle = (float)j * 2 * glm::pi<float>() / sectorCount;
      float x = xy * cosf(sectorAngle);
      float y = xy * sinf(sectorAngle);

      vec3 pos = vec3(x, y, z);
      vec3 norm = glm::normalize(pos);
      vec2 tex = vec2((float)j / sectorCount, (float)i / stackCount);

      vertexArray.position[index] = pos;
      vertexArray.prevPos[index] = pos; // can initialize same as position
      vertexArray.normal[index] = norm;
      vertexArray.force[index] = vec3(0.0f);
      vertexArray.texCoords[index] = tex;
    }
  }

  return vertexArray;
}

auto Spheres::generateIndices() -> vector<GLuint> {

  std::vector<GLuint> indices;
  indices.reserve(sectorCount * stackCount * 6);

  for (unsigned int i = 0; i < stackCount; ++i) {
    for (unsigned int j = 0; j < sectorCount; ++j) {
      unsigned int topLeft = i * (sectorCount + 1) + j;
      unsigned int topRight = topLeft + 1;
      unsigned int bottomLeft = (i + 1) * (sectorCount + 1) + j;
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

///*************SPHERE CLASS**************///
