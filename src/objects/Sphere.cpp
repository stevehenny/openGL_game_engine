
#include "Sphere.h"
#include <cmath>
#include <glm/gtc/constants.hpp>

std::vector<Vertex> generateSphere(float radius, unsigned int sectorCount,
                                   unsigned int stackCount) {
  std::vector<Vertex> vertices;

  for (unsigned int i = 0; i <= stackCount; ++i) {
    float stackAngle =
        glm::pi<float>() / 2 -
        (float)i * glm::pi<float>() / stackCount; // from pi/2 to -pi/2
    float xy = radius * cosf(stackAngle);
    float z = radius * sinf(stackAngle);

    for (unsigned int j = 0; j <= sectorCount; ++j) {
      float sectorAngle = (float)j * 2 * glm::pi<float>() / sectorCount;

      float x = xy * cosf(sectorAngle);
      float y = xy * sinf(sectorAngle);

      float u = (float)j / sectorCount;
      float v = (float)i / stackCount;

      Vertex vertex;
      vertex.position = glm::vec3(x, y, z);
      vertex.normal = glm::normalize(glm::vec3(x, y, z));
      vertex.texCoords = glm::vec2(u, v);
      vertices.push_back(vertex);
    }
  }

  // Generate indices for GL_TRIANGLES
  std::vector<Vertex> sphereTriangles;
  for (unsigned int i = 0; i < stackCount; ++i) {
    unsigned int k1 = i * (sectorCount + 1);
    unsigned int k2 = k1 + sectorCount + 1;

    for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
      if (i != 0) {
        sphereTriangles.push_back(vertices[k1]);
        sphereTriangles.push_back(vertices[k2]);
        sphereTriangles.push_back(vertices[k1 + 1]);
      }

      if (i != (stackCount - 1)) {
        sphereTriangles.push_back(vertices[k1 + 1]);
        sphereTriangles.push_back(vertices[k2]);
        sphereTriangles.push_back(vertices[k2 + 1]);
      }
    }
  }

  return sphereTriangles;
}
