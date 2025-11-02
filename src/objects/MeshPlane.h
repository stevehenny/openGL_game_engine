#pragma once
#include "Common.h"
#include "Object.h"
#include "Shader.h"
#include "glad/glad.h"
#include "top_level_include.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

using glm::vec3, std::vector;
class MeshPlane : public Object {

public:
  MeshPlane(double width, double depth, unsigned int xSegments,
            unsigned int zSegments, float yPos, float damping);
  ~MeshPlane();
  void draw() override;
  void update();
  vec3 getLocation() const;
  Shader &getShader() const;
  void updateSBBO(vector<GravityObject> &objects) const;

private:
  auto generateVertices(double width, double depth) -> vector<Vertex>;
  auto generateLineIndices() -> vector<unsigned int>;

  auto generateTriIndices() -> vector<unsigned int>;

  auto generatePlaneConstraints(vector<Vertex> &planeVertices)
      -> vector<PlaneConstraint>;

  vector<Vertex> planeVertices;
  vector<unsigned int> planeTriIndices;  // used for tracking effects
  vector<unsigned int> planeLineIndices; // used to draw
  vector<PlaneConstraint> planeConstraints;
  double width, depth;
  unsigned int xSegments, zSegments;
  float yPos;
  float damping;
  class Impl;
  std::unique_ptr<Impl> resources;
};
