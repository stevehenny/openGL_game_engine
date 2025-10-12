add_library(objects Shape.cpp Triangle.cpp Square.cpp CameraObject.cpp)
include_directories(${SOURCE_DIR}/shaders)
target_link_libraries(objects PRIVATE glm::glm)
