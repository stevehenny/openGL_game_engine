add_library(objects Shape.cpp Triangle.cpp Square.cpp Camera.cpp Cube.cpp Sphere.cpp MeshPlane.cpp Object.cpp)
target_link_libraries(objects PRIVATE glm::glm shaders)
