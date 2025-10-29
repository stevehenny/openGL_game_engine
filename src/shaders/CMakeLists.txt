add_library(shaders
  Shader.cpp ShaderProgram.cpp ShaderPrograms.cpp
)

set (SHADER_OUPUT_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/spv")
file(MAKE_DIRECTORY ${SHADER_OUPUT_FOLDER})

file(GLOB SHADER_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.vert"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
)

set (SPV_OUTPUTS "")

foreach(SHADER_FILE ${SHADER_SOURCES})
  get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
  set(SPV_FILE "${SHADER_OUPUT_FOLDER}/${FILE_NAME}.spv")

  add_custom_command(
    OUTPUT ${SPV_FILE}
    COMMAND glslangValidator -G ${SHADER_FILE} -o ${SPV_FILE}
    DEPENDS ${SHADER_FILE}
    COMMENT "Compiling Shader: ${SHADER_FILE}"
    VERBATIM
  )

  list(APPEND SPV_OUTPUTS ${SPV_FILE})

endforeach()

add_custom_target(compile_shaders ALL 
  DEPENDS ${SPV_OUTPUTS}
)


target_link_libraries(shaders PRIVATE glm::glm)

