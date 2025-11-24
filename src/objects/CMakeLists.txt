add_library(objects Shape.cpp Triangle.cpp Square.cpp Camera.cpp Cube.cpp Spheres.cpp MeshPlane.cpp Object.cpp Particle.cpp)
target_link_libraries(objects PRIVATE glm::glm shaders)
