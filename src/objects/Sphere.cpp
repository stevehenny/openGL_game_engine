
#include "Sphere.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using glm::vec3, glm::vec2;

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

std::vector<Vertex> generatePlane(float width, float depth,
                                  unsigned int xSegments,
                                  unsigned int zSegments, float yPos) {
  std::vector<Vertex> vertices;
  vertices.reserve((xSegments + 1) * (zSegments + 1));

  for (unsigned int i = 0; i <= zSegments; ++i) {
    float z = depth * (static_cast<float>(i) / zSegments - 0.5f);
    for (unsigned int j = 0; j < xSegments; ++j) {
      float x = width * (static_cast<float>(j) / xSegments - 0.5f);

      Vertex v;
      v.position = vec3(x, yPos, z);
      v.normal = vec3(0.0f, 1.0f, 0.0f);
      v.texCoords = vec2(static_cast<float>(j) / xSegments,
                         static_cast<float>(x) / zSegments);

      vertices.push_back(v);
    }
  }

  return vertices;
}

std::vector<unsigned int> generateIndices(unsigned int xSegments,
                                          unsigned int zSegments) {
  std::vector<unsigned int> indices;
  for (unsigned int i = 0; i <= zSegments; ++i) {
    for (unsigned int j = 0; j <= xSegments; ++j) {
      unsigned int topLeft = i * (xSegments + 1) + j;
      unsigned int topRight = topLeft + 1;
      unsigned int bottomLeft = (i + 1) * (xSegments + 1) + j;
      unsigned int bottomRight = bottomLeft + 1;

      // first triangle
      indices.push_back(topLeft);
      indices.push_back(bottomLeft);
      indices.push_back(topRight);

      // second triangle
      indices.push_back(topRight);
      indices.push_back(bottomLeft);
      indices.push_back(bottomRight);
    }
  }
  return indices;
}
