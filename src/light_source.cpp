#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "window_functions.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using glm::vec3, glm::mat4, std::cout;
static constexpr unsigned int SCR_WIDTH = 1920;
static constexpr unsigned int SCR_HEIGHT = 1080;
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
static vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
static vec3 lightPos2 = vec3(-1.2, -1.0f, -2.0f);

int arr[15];
std::array<int, 15> arr2;

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
static constexpr glm::vec3 cube1_pos = glm::vec3(0.0f, -1.0f, 1.0f);
int main(int argc, char *argv[]) {

  if (argc < 4) {
    fprintf(stderr, "Usage: %s <vertex_shader_path> <fragment_shader_path> ",
            argv[0]);
    return -1;
  }
  const char *vertexShaderPath = argv[1];
  const char *fragmentShaderPath = argv[2];
  const char *lightfragPath = argv[3];
  const char *lightVertPath = "../../src/shaders/light_source.vert";
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                        "LIGHTING_PRACITCE", nullptr, nullptr);

  Camera camera{vec3(0.0f, 0.0f, 3.0f),
                vec3(0.0f, 0.0f, -1.0f),
                vec3(0.0f, 1.0f, 0.0f),
                1920.0f,
                1080.0f,
                0.1f,
                0.1f,
                100.0f,
                2.5f};

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwFocusWindow(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetWindowUserPointer(window, &camera);
  glfwSetCursorPosCallback(window, Camera::mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Faild to initialized GLAD" << '\n';
    return -1;
  }

  // create cube buffers
  unsigned int cubeVAO, VBO;
  glGenVertexArrays(1, &cubeVAO);

  glBindVertexArray(cubeVAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  unsigned int lightVAO2;
  glGenVertexArrays(1, &lightVAO2);
  glBindVertexArray(lightVAO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0); // enable depth
  glEnable(GL_DEPTH_TEST);

  // compile shaders
  Shader shader = Shader(vertexShaderPath, fragmentShaderPath);
  Shader lightShader = Shader(lightVertPath, lightfragPath);
  Shader lightShader2 = Shader(lightVertPath, lightfragPath);
  shader.useShader();

  float rotationRadius = 2.0f;
  float offset = 1.0f;
  vec3 ColorVec = vec3(0.5f, 0.5f, 0.5f);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    float time = glfwGetTime();

    camera.update_camera_delta_time(time);
    camera.move_camera(window);

    float lightX = sin(time) * rotationRadius;
    float lightY = 1.0f;
    float lightZ = cos(time) * rotationRadius;

    float lightX2 = cos(time) * rotationRadius;
    float lightY2 = cos(time) * rotationRadius;
    float lightZ2 = sin(time) * rotationRadius;

    lightPos = vec3(lightX, lightY, lightZ);
    lightPos2 = vec3(lightX2, lightY2, lightZ2);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.useShader();
    shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    shader.setVec3("lightColor", vec3(1.0f, 0.0f, 0.0f));
    shader.setVec3("lightColor2", vec3(0.0f, 0.0f, 1.0f));
    shader.setVec3("lightPos2", lightPos2);
    shader.setMat4("projection", camera.get_projection_matrix());
    shader.setMat4("view", camera.get_view_matrix());
    shader.setMat4("model", mat4(1.0f));
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("viewPos", camera.get_position());
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    lightShader.useShader();
    lightShader.setMat4("projection", camera.get_projection_matrix());
    lightShader.setMat4("view", camera.get_view_matrix());
    lightShader.setVec3("lightColor", vec3(1.0f, 0.0f, 0.0f));
    mat4 model = mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, vec3(0.2f));
    lightShader.setMat4("model", model);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw second light
    lightShader2.useShader();
    lightShader2.setMat4("projection", camera.get_projection_matrix());
    lightShader2.setMat4("view", camera.get_view_matrix());
    lightShader2.setVec3("lightColor", vec3(1.0f, 1.0f, 1.0f));
    mat4 model2 = mat4(1.0f);
    model2 = glm::translate(model2, lightPos2);
    model2 = glm::scale(model2, vec3(0.5f));
    lightShader2.setMat4("model", model2);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
  }
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();
  return 0;
}
