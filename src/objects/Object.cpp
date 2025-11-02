#include "Object.h"
#include "top_level_include.h"
#include <iostream>

// This method will check to verify a a object is not being
// created before the global gl state is ready to run
Object::Object() {
  // Ensure glGetString is valid before calling
  if (glGetString == nullptr) {
    std::cerr
        << "[ERROR] GLAD not initialized — glGetString pointer is null.\n";
    throw std::runtime_error("GLAD not initialized before Object creation");
  }

  const GLubyte *version = glGetString(GL_VERSION);
  if (version == nullptr) {
    std::cerr << "[ERROR] No valid OpenGL context when creating Object.\n";
    throw std::runtime_error(
        "OpenGL context not active before Object creation");
  }
}
