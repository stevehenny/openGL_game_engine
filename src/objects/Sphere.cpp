
#include "Sphere.h"
#include <cmath>

std::vector<Vertex> generateSphere(float radius, int sectorCount,
                                   int stackCount) {
  std::vector<Vertex> vertices;

  float PI = 3.14159265359f;
  for (int i = 0; i <= stackCount; ++i) {
    float stackAngle = PI / 2 - i * PI / stackCount; // from pi/2 to -pi/2
    float xy = radius * cosf(stackAngle);
    float z = radius * sinf(stackAngle);

    for (int j = 0; j <= sectorCount; ++j) {
      float sectorAngle = j * 2 * PI / sectorCount; // from 0 to 2pi
      float x = xy * cosf(sectorAngle);
      float y = xy * sinf(sectorAngle);
      float nx = x / radius;
      float ny = y / radius;
      float nz = z / radius;
      vertices.push_back({x, y, z, nx, ny, nz});
    }
  }

  std::vector<Vertex> sphereVertices;
  for (int i = 0; i < stackCount; ++i) {
    int k1 = i * (sectorCount + 1);
    int k2 = k1 + sectorCount + 1;

    for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
      if (i != 0) {
        sphereVertices.push_back(vertices[k1]);
        sphereVertices.push_back(vertices[k2]);
        sphereVertices.push_back(vertices[k1 + 1]);
      }
      if (i != (stackCount - 1)) {
        sphereVertices.push_back(vertices[k1 + 1]);
        sphereVertices.push_back(vertices[k2]);
        sphereVertices.push_back(vertices[k2 + 1]);
      }
    }
  }
  return sphereVertices;
}
