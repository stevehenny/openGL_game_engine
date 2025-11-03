#include "Sphere.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using glm::vec3, glm::vec2;

// -------------------- SPHERE --------------------
std::vector<Vertex> generateSphere(float radius, unsigned int sectorCount,
                                   unsigned int stackCount) {
  std::vector<Vertex> vertices;

  VertexArray vertexArray;
  for (unsigned int i = 0; i <= stackCount; ++i) {
    float stackAngle =
        glm::pi<float>() / 2 - (float)i * glm::pi<float>() / stackCount;
    float xy = radius * cosf(stackAngle);
    float z = radius * sinf(stackAngle);

    for (unsigned int j = 0; j <= sectorCount; ++j) {
      float sectorAngle = (float)j * 2 * glm::pi<float>() / sectorCount;
      float x = xy * cosf(sectorAngle);
      float y = xy * sinf(sectorAngle);

      Vertex vertex;
      vertex.position = glm::vec3(x, y, z);
      vertex.normal = glm::normalize(glm::vec3(x, y, z));
      vertex.texCoords =
          glm::vec2((float)j / sectorCount, (float)i / stackCount);
      vertices.push_back(vertex);
    }
  }

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

VertexArray generateVertexArray(float radius, unsigned int sectorCount,
                                unsigned int stackCount) {
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

// -------------------- PLANE --------------------
std::vector<Vertex> generatePlane(float width, float depth,
                                  unsigned int xSegments,
                                  unsigned int zSegments, float yPos) {
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

// -------------------- TRIANGLE INDICES --------------------
std::vector<unsigned int> generateIndices(unsigned int xSegments,
                                          unsigned int zSegments) {
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

std::vector<unsigned int> generateGridLineIndices(unsigned int xSegments,
                                                  unsigned int zSegments) {
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

// -------------------- CONSTRAINTS --------------------
std::vector<PlaneConstraint>
generatePlaneConstraints(std::vector<Vertex> &planeVertices,
                         unsigned int xSegments, unsigned int zSegments) {
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
