#include "Particle.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderPrograms.h"
#include "top_level_include.h"
#include <cstdint>
#include <memory>
#include <random>

using glm::vec4, std::vector;
class Particles::Impl {

public:
  Impl()
      : computeShader(Shader(ShaderProgram{compiled_shaders::WAVE_FUNCTION_COMP,
                                           ShaderTypes::COMPUTE})) {
    glGenBuffers(1, &ParticleVAO);
    glGenBuffers(1, &ParticleSBBO);
  }

  ~Impl() {
    glDeleteBuffers(1, &ParticleVAO);
    glDeleteBuffers(1, &ParticleSBBO);
  }
  GLuint ParticleVAO, ParticleSBBO;
  Shader computeShader;
};

Particles::~Particles() = default;

// keep method blank.
void Particles::draw() {}

Particles::Particles(uint32_t num_particles)
    : resources(std::make_unique<Impl>()) {

  this->particles = generateParticles(num_particles);

  // Upload initial particle buffer
  glBindVertexArray(resources->ParticleVAO);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, resources->ParticleSBBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * particles.size(),
               particles.data(), GL_DYNAMIC_DRAW);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, resources->ParticleSBBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Particles::evolve() {
  resources->computeShader.useShader();

  // Example uniforms
  resources->computeShader.setFloat(
      glGetUniformLocation(resources->computeShader.ID, "dt"), dt);
  resources->computeShader.setUInt(
      glGetUniformLocation(resources->computeShader.ID, "numParticles"),
      particles.size());

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, resources->ParticleSBBO);

  GLuint groups = (particles.size() + 255) / 256;
  glDispatchCompute(groups, 1, 1);

  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Particles::measure() {
  // Replace with collapse algorithm
  for (auto &p : particles)
    p.collapsed = true;
}

void Particles::updateProbabilities() {
  for (auto &p : particles)
    p.probability = std::norm(p.amplitude);
}

vector<Particle> Particles::generateParticles(uint32_t num_particles) {
  vector<Particle> p(num_particles);

  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> unif(0.0f, 1.0f);

  for (auto &x : p) {
    x.position = vec4(unif(rng), unif(rng), unif(rng), 1.0f);
    x.velocity = vec4(0.0f);
    x.amplitude = {1.0f, 0.0f};
    x.probability = 1.0f;
    x.collapsed = false;
  }

  return p;
}
