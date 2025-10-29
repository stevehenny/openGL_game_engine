#pragma once
#include <vector>

struct Vertex {
  float x, y, z;
  float nx, ny, nz;
};

std::vector<Vertex> generateSphere(float radius, int sectorCount,
                                   int stackCount);
