add_library(objects Shape.cpp Triangle.cpp Square.cpp Camera.cpp)
target_link_libraries(objects PRIVATE glm::glm shaders)
