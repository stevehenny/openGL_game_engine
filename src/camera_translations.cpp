#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "window_functions.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stb_image.h>

// Globals (kept from your original file)
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float camera_yaw = -90.0f;
float camera_pitch = 0.0f;
float lastX = 1920.0f / 2.0;
float lastY = 1080.0f / 2.0;
float fov = 45.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

using namespace std;

constexpr unsigned int SCR_WIDTH = 1920;
constexpr unsigned int SCR_HEIGHT = 1080;

// Uniform / binding numeric locations that must match your shaders:
// (from the vertex shader you posted)
// layout(location = 2) uniform mat4 projection;
// layout(location = 3) uniform mat4 view;
// layout(location = 4) uniform mat4 model;
// (from the fragment shader)
// layout(binding = 0) uniform sampler2D outTexture;
constexpr unsigned int PROJECTION_LOC = 2u;
constexpr unsigned int VIEW_LOC = 3u;
constexpr unsigned int MODEL_LOC = 4u;
constexpr unsigned int OUTTEXTURE_LOC = 0u;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr,
            "Usage: %s <vertex_shader_path> <fragment_shader_path> "
            "<texture_path>\n",
            argv[0]);
    return -1;
  }

  const char *vertexShaderPath = argv[1];
  const char *fragmentShaderPath = argv[2];
  const char *texturePath = argv[3];

  // glfw init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  Camera camera{glm::vec3(0.0f, 0.0f, 3.0f),
                glm::vec3(0.0f, 0.0f, -1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f),
                1920.0f,
                1080.0f,
                0.1f,
                0.1f,
                100.0f,
                2.5f};

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetWindowUserPointer(window, &camera);
  glfwSetCursorPosCallback(window, Camera::mouse_callback);
  glfwSetScrollCallback(window, Camera::scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Build and link shader program from files (your ShaderProgram class handles
  // compilation)
  Shader ourShader(
      ShaderProgram{std::string(vertexShaderPath), ShaderTypes::VERTEX},
      ShaderProgram{std::string(fragmentShaderPath), ShaderTypes::FRAGMENT});

  // Cube positions (unchanged)
  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  // vertex data (positions + texcoords)
  float vertices[] = {
      // positions          // tex coords
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  // VAO/VBO
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute (location = 0 in vertex shader)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // texcoord attribute (location = 1 in vertex shader)
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // load texture
  unsigned int texture1 = loadTexture(texturePath);

  // Make sure our shader knows which texture binding to sample (binding = 0)
  ourShader.useShader();
  // ourShader.setInt(OUTTEXTURE_LOC, 0); // sampler at binding 0 -> texture
  // unit 0

  glEnable(GL_DEPTH_TEST);

  // Construct cube helper (Cube will set model matrix per-instance)
  Cube cube{glm::vec3(0.0f, 0.0f, 0.0f), texture1, ourShader};

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    // update camera timing
    camera.update_camera_delta_time(glfwGetTime());
    camera.move_camera(window);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind texture unit 0 (matches layout(binding = 0))
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set view and projection (numeric locations)
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix();

    ourShader.useShader();
    ourShader.setMat4(VIEW_LOC, view);
    ourShader.setMat4(PROJECTION_LOC, projection);
    // sampler already set to 0 earlier; setting again is harmless
    // ourShader.setInt(OUTTEXTURE_LOC, 0);

    // draw cubes
    for (int i = 0; i < 10; i++) {
      glBindVertexArray(VAO);
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, (float)glfwGetTime() * (i + 1),
                          glm::vec3((i + 1) * 0.1f, (i + 1) * 0.02f, 0.5f));
      // numeric location for model
      ourShader.useShader();
      ourShader.setMat4(MODEL_LOC, model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}
