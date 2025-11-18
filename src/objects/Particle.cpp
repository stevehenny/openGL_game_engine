#include "Particle.h"
#include "Shader.h"
#include "top_level_include.h"
#include <cstdint>
#include <random>

using glm::vec4, std::vector;
class Particles::Impl {

public:
  void initializeParticles();
  Impl() : computeShader(Shader()) {
    glGenBuffers(1, &ParticleVAO);
    // glGenBuffers(1, &ParticleVBO);
    glGenBuffers(1, &ParticleSBBO);

    initializeParticles();
  }

  ~Impl() {
    glDeleteBuffers(1, &ParticleVAO);
    // glDeleteBuffers(1, &ParticleVBO);
    glDeleteBuffers(1, &ParticleSBBO);
  }
  GLuint ParticleVAO, ParticleSBBO;
  Shader computeShader;
};

Particles::Particles(uint32_t num_particles) {

  this->particles = generateParticles(num_particles);
  // bind VAO
  glBindVertexArray(resources->ParticleVAO);

  // setup SBBO
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, resources->ParticleSBBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER,
               sizeof(Particle) * this->particles.size(),
               this->particles.data(), GL_DYNAMIC_DRAW);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, resources->ParticleSBBO);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

Particles::~Particles() = default;

void Particles::draw() {}

void Particles::evolve() {

  resources->computeShader.useShader();

  // TODO: set uniforms

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, resources->ParticleSBBO);
  GLuint groups = (particles.size() + 255) / 256;
  glDispatchCompute(groups, 1, 1);

  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Particles::measure() {}

void Particles::updateProbabilities() {}

vector<Particle> Particles::generateParticles(uint32_t num_particles) {
  vector<Particle> particles(num_particles);
  std::mt19937 random_engine;
  std::uniform_real_distribution<float> uniform(0, 1);
  for (uint32_t i{}; i < num_particles; ++i) {
    particles[i].position =
        vec4(uniform(random_engine), 1.0f, uniform(random_engine), 1.0f);
    particles[i].velocity = vec4(0.0f);
    particles[i].amplitude = std::complex<float>{1.0f, 0.0f};
    particles[i].collapsed = false;
  }

  return particles;
}
