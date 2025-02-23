add_library(objects Shape.cpp Triangle.cpp)


target_include_directories(objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
