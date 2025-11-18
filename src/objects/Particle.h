#include "Object.h"
#include <complex>
#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
using glm::vec4, std::vector;

struct Particle {
  glm::vec4 position;            // Position in 3D space
  glm::vec4 velocity;            // Optional, for dynamics
  std::complex<float> amplitude; // ψ at this position
  float probability;             // |ψ|^2
  bool collapsed;                // measurement has occured
};
class Particles : public Object {

public:
  Particles(uint32_t num_particles);
  ~Particles();

  void draw() override;
  void evolve();
  void measure();
  void updateProbabilities(); // compute |ψ|^2
  vector<Particle> generateParticles(uint32_t num_particles);

private:
  vector<Particle> particles;
  glm::vec3 dimensions;
  float dt;
  vector<std::complex<float>> psi; // wavefunction on a grid
  class Impl;
  std::unique_ptr<Impl> resources;
};
