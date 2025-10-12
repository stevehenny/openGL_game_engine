add_library(textures Texture.cpp)

target_include_directories(textures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(textures PRIVATE glm::glm)
