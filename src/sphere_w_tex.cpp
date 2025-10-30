#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderPrograms.h"
#include "Sphere.h"
#include "Texture.h"
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

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Incorrect number of params\n";
    exit(1);
  }
  char *texturePath = argv[1];
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

  // Generate sphere
  auto sphereVerts = generateSphere(0.5f, 36, 18);

  unsigned int sphereVAO, sphereVBO;
  glGenVertexArrays(1, &sphereVAO);
  glGenBuffers(1, &sphereVBO);
  glBindVertexArray(sphereVAO);
  glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
  glBufferData(GL_ARRAY_BUFFER, sphereVerts.size() * sizeof(Vertex),
               sphereVerts.data(), GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);

  // normal
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);

  // texcoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(offsetof(Vertex, texCoords)));
  glEnableVertexAttribArray(2);

  // Plane VAO/VBO
  static constexpr float planeVertices[] = {
      // positions          // normals
      -5.0f, 0.0f, -5.0f, 0.0f,  1.0f, 0.0f,  5.0f,  0.0f, -5.0f,
      0.0f,  1.0f, 0.0f,  5.0f,  0.0f, 5.0f,  0.0f,  1.0f, 0.0f,
      5.0f,  0.0f, 5.0f,  0.0f,  1.0f, 0.0f,  -5.0f, 0.0f, 5.0f,
      0.0f,  1.0f, 0.0f,  -5.0f, 0.0f, -5.0f, 0.0f,  1.0f, 0.0f};

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

  glEnable(GL_DEPTH_TEST);

  Shader shader{ShaderProgram{compiled_shaders::OBJECT_W_TEXTURE_VERT,
                              ShaderTypes::VERTEX},
                ShaderProgram{compiled_shaders::OBJECT_W_TEXTURE_FRAG,
                              ShaderTypes::FRAGMENT}};

  Shader lightShader{
      ShaderProgram{compiled_shaders::LIGHT_VERTEX, ShaderTypes::VERTEX},
      ShaderProgram{compiled_shaders::LIGHT_FRAG, ShaderTypes::FRAGMENT}};

  Shader lightShader2{
      ShaderProgram{compiled_shaders::LIGHT_VERTEX, ShaderTypes::VERTEX},
      ShaderProgram{compiled_shaders::LIGHT_FRAG, ShaderTypes::FRAGMENT}};

  shader.useShader();

  unsigned int sphereTexture;
  glGenTextures(1, &sphereTexture);
  glBindTexture(GL_TEXTURE_2D, sphereTexture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  sphereTexture = loadTexture(texturePath);

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

    // Draw sphere
    shader.useShader();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sphereTexture);
    shader.setInt(6, 0);
    shader.setVec3(5, vec3(1.f, 1.f, 1.f));            // objectColor
    shader.setVec3(7, vec3(1.f, 0.f, 0.f));            // lightColor
    shader.setVec3(9, vec3(1.f, 1.f, 1.f));            // lightColor2
    shader.setVec3(10, lightPos2);                     // lightPos2
    shader.setMat4(4, camera.get_projection_matrix()); // projection
    shader.setMat4(3, camera.get_view_matrix());       // view
    shader.setMat4(2, mat4(1.0f));                     // model
    shader.setVec3(8, lightPos);                       // lightPos
    shader.setVec3(11, camera.get_position());         // viewPos
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sphereVerts.size()));

    // Draw plane
    mat4 planeModel = mat4(1.0f);
    planeModel = glm::translate(planeModel, vec3(0.0f, -3.0f, 0.0f));
    shader.setMat4(2, planeModel);
    shader.setVec3(5, 0.5f, 0.5f, 0.5f);
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
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sphereVerts.size()));

    // Draw second light
    lightShader2.useShader();
    lightShader2.setMat4(4, camera.get_projection_matrix());
    lightShader2.setMat4(3, camera.get_view_matrix());
    lightShader2.setVec3(5, 1.f, 1.f, 1.f);
    mat4 model2 = mat4(1.f);
    model2 = glm::translate(model2, lightPos2);
    model2 = glm::scale(model2, vec3(0.5f));
    lightShader2.setMat4(2, model2);
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sphereVerts.size()));

    glfwSwapBuffers(window);
  }

  glDeleteVertexArrays(1, &sphereVAO);
  glDeleteVertexArrays(1, &planeVAO);
  glDeleteBuffers(1, &sphereVBO);
  glDeleteBuffers(1, &planeVBO);

  glfwTerminate();
  return 0;
}
