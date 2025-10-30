# Create a library for your shader-related C++ source files
add_library(shaders
  Shader.cpp
  ShaderProgram.cpp
  ShaderPrograms.cpp
)

# Folder to store compiled SPIR-V shader binaries
set(SHADER_OUTPUT_FOLDER "${CMAKE_CURRENT_BINARY_DIR}/spv")
file(MAKE_DIRECTORY ${SHADER_OUTPUT_FOLDER})

# Collect all shader source files (*.vert, *.frag)
file(GLOB SHADER_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.vert"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.frag"
)

set(SPV_OUTPUTS "")

# Compile each GLSL shader into SPIR-V with glslangValidator
foreach(SHADER_FILE ${SHADER_SOURCES})
  get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
  set(SPV_FILE "${SHADER_OUTPUT_FOLDER}/${FILE_NAME}.spv")

  add_custom_command(
    OUTPUT ${SPV_FILE}
    COMMAND glslangValidator -G ${SHADER_FILE} -o ${SPV_FILE}
    DEPENDS ${SHADER_FILE}
    COMMENT "Compiling Shader: ${FILE_NAME}"
    VERBATIM
  )

  list(APPEND SPV_OUTPUTS ${SPV_FILE})
endforeach()

# Create a custom target that depends on all compiled shader outputs
add_custom_target(compile_shaders ALL
  DEPENDS ${SPV_OUTPUTS}
  COMMENT "All shaders compiled to SPIR-V"
)

# Make sure the 'shaders' library builds only after shaders are compiled
add_dependencies(shaders compile_shaders)

# Link GLM to the shaders library
target_link_libraries(shaders PRIVATE glm::glm)
