#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "window_functions.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>

using glm::mat4;
using glm::vec3;

static constexpr unsigned int SCR_WIDTH = 1920;
static constexpr unsigned int SCR_HEIGHT = 1080;

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
static vec3 lightPos2 = vec3(-1.2f, -1.0f, -2.0f);

static constexpr float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,
    -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,
    0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
    0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,
    0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,
    0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
    0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  0.5f,  0.5f,
    0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,
    -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
    0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,
    -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f,
    -0.5f, 0.0f,  -1.0f, 0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,
    0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,
    0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

static constexpr vec3 cube1_pos = vec3(0.0f, -1.0f, 1.0f);
static constexpr float planeVertices[] = {
    // positions          // normals
    -5.0f, 0.0f, -5.0f, 0.0f,  1.0f, 0.0f,  5.0f,  0.0f, -5.0f,
    0.0f,  1.0f, 0.0f,  5.0f,  0.0f, 5.0f,  0.0f,  1.0f, 0.0f,

    5.0f,  0.0f, 5.0f,  0.0f,  1.0f, 0.0f,  -5.0f, 0.0f, 5.0f,
    0.0f,  1.0f, 0.0f,  -5.0f, 0.0f, -5.0f, 0.0f,  1.0f, 0.0f};
int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr,
            "Usage: %s <vertex_shader> <fragment_shader> <light_frag>\n",
            argv[0]);
    return -1;
  }

  const char *vertexShaderPath = argv[1];
  const char *fragmentShaderPath = argv[2];
  const char *lightFragPath = argv[3];
  const char *lightVertPath = "../../src/shaders/light_source.vert";

  // GLFW setup
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                        "LIGHTING_PRACTICE", nullptr, nullptr);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  Camera camera{vec3(0.0f, 0.0f, 3.0f),
                vec3(0.f, 0.f, -1.f),
                vec3(0.f, 1.f, 0.f),
                (float)SCR_WIDTH,
                (float)SCR_HEIGHT,
                0.1f,
                0.1f,
                100.f,
                2.5f};

  glfwSetWindowUserPointer(window, &camera);
  glfwSetCursorPosCallback(window, Camera::mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Cube VAO/VBO
  unsigned int cubeVAO, cubeVBO;
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Plane VAO/VBO
  unsigned int planeVAO, planeVBO;
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);
  glBindVertexArray(planeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Light VAOs
  unsigned int lightVAO, lightVAO2;
  glGenVertexArrays(1, &lightVAO);
  glGenVertexArrays(1, &lightVAO2);

  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(lightVAO2);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
  glEnableVertexAttribArray(0);

  glEnable(GL_DEPTH_TEST);

  // Compile SPIR-V shaders
  Shader shader{ShaderProgram{vertexShaderPath, ShaderTypes::VERTEX},
                ShaderProgram{fragmentShaderPath, ShaderTypes::FRAGMENT}};

  Shader lightShader{ShaderProgram{lightVertPath, ShaderTypes::VERTEX},
                     ShaderProgram{lightFragPath, ShaderTypes::FRAGMENT}};

  Shader lightShader2{ShaderProgram{lightVertPath, ShaderTypes::VERTEX},
                      ShaderProgram{lightFragPath, ShaderTypes::FRAGMENT}};

  shader.useShader();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    float time = glfwGetTime();
    camera.update_camera_delta_time(time);
    camera.move_camera(window);

    // Animate lights
    lightPos = vec3(sin(time) * 2.0f, 1.0f, cos(time) * 2.0f);
    lightPos2 = vec3(cos(time) * 2.0f, cos(time) * 2.0f, sin(time) * 2.0f);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw cube
    shader.useShader();
    shader.setVec3(5, 1.f, 1.f, 1.f);                  // objectColor
    shader.setVec3(6, 1.f, 0.f, 0.f);                  // lightColor
    shader.setVec3(8, 0.f, 0.f, 1.f);                  // lightColor2
    shader.setVec3(9, lightPos2);                      // lightPos2
    shader.setMat4(4, camera.get_projection_matrix()); // projection
    shader.setMat4(3, camera.get_view_matrix());       // view
    shader.setMat4(2, mat4(1.0f));                     // model
    shader.setVec3(7, lightPos);                       // lightPos
    shader.setVec3(10, camera.get_position());         // viewPos
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw plane (floor/wall)
    mat4 planeModel = mat4(1.0f);
    planeModel = glm::translate(planeModel, vec3(0.0f, -3.0f, 0.0f));
    shader.setMat4(2, planeModel);
    shader.setVec3(5, 0.5f, 0.5f, 0.5f); // objectColor for plane
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw first light
    lightShader.useShader();
    lightShader.setMat4(4, camera.get_projection_matrix());
    lightShader.setMat4(3, camera.get_view_matrix());
    lightShader.setVec3(5, 1.f, 0.f, 0.f);
    mat4 model = mat4(1.f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, vec3(0.2f));
    lightShader.setMat4(2, model);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw second light
    lightShader2.useShader();
    lightShader2.setMat4(4, camera.get_projection_matrix());
    lightShader2.setMat4(3, camera.get_view_matrix());
    lightShader2.setVec3(5, 1.f, 1.f, 1.f);
    mat4 model2 = mat4(1.f);
    model2 = glm::translate(model2, lightPos2);
    model2 = glm::scale(model2, vec3(0.5f));
    lightShader2.setMat4(2, model2);
    glBindVertexArray(lightVAO2);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteVertexArrays(1, &lightVAO2);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteBuffers(1, &planeVBO);

  glfwTerminate();
  return 0;
}
