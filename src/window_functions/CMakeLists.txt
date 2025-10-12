add_library(window_functions window_functions.cpp)


target_include_directories(window_functions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(window_functions PRIVATE glm::glm)
