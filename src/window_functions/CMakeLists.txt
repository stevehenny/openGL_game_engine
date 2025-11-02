add_library(window_functions window_functions.cpp TextRender.cpp)


target_link_libraries(window_functions PRIVATE glm::glm)
