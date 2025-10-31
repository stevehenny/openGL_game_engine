#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderPrograms.h"
#include "Sphere.h"
#include "Texture.h"
#include "window_functions.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>

using glm::mat4;
using glm::vec3;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Color {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

static constexpr unsigned int SCR_WIDTH = 1920;
static constexpr unsigned int SCR_HEIGHT = 1080;

static vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
static vec3 lightPos2 = vec3(-1.2f, -1.0f, -2.0f);

int main(int argc, char *argv[]) {
  if (argc < 5) {
    std::cerr << "Usage: ./lighting <texture1> <texture2> <texture3>\n";
    return -1;
  }

  char *texturePath = argv[1];
  char *texture2Path = argv[2];
  char *texture3Path = argv[3];
  char *texture4Path = argv[4];

  // GLFW setup
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                        "LIGHTING_PRACTICE", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
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
    std::cerr << "Failed to initialize GLAD\n";
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)(offsetof(Vertex, texCoords)));
  glEnableVertexAttribArray(2);

  // Plane VAO/VBO
  // float planeVertices[] = {
  //     // positions            // normals          // texCoords
  //     -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
  //     5.0f,  0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
  //     5.0f,  0.0f, 5.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
  //
  //     -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
  //     5.0f,  0.0f, 5.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
  //     -5.0f, 0.0f, 5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top-left
  // };
  std::vector<Vertex> planeVertices =
      generatePlane(10.0f, 10.0f, 100u, 100u, 3.0f);
  std::vector<unsigned int> planeIndices = generateIndices(100u, 100u);

  // Move the plane up
  // const float height = 2.5f;
  // for (size_t i = 0; i < sizeof(planeVertices) / sizeof(float); i += 8)
  //   planeVertices[i + 1] += height; // increment Y component

  unsigned int planeVAO, planeVBO, planeEBO;
  glGenVertexArrays(1, &planeVAO);
  glGenBuffers(1, &planeVBO);
  glGenBuffers(1, &planeEBO);

  // Attribute buffer for plane
  glBindVertexArray(planeVAO);

  // bind VBO vertices
  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
  glBufferData(GL_ARRAY_BUFFER, planeVertices.size() * sizeof(Vertex),
               planeVertices.data(), GL_STATIC_DRAW);

  // bind EBO indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               planeIndices.size() * sizeof(unsigned int), planeIndices.data(),
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);

  // texCoord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<void *>(offsetof(Vertex, texCoords)));
  glEnableVertexAttribArray(2);

  glEnable(GL_DEPTH_TEST);

  Shader shader{ShaderProgram{compiled_shaders::OBJECT_W_TEXTURE_VERT,
                              ShaderTypes::VERTEX},
                ShaderProgram{compiled_shaders::OBJECT_W_TEXTURE_FRAG,
                              ShaderTypes::FRAGMENT}};

  Shader lightShader{ShaderProgram{compiled_shaders::LIGHT_W_TEXTURE_VERT,
                                   ShaderTypes::VERTEX},
                     ShaderProgram{compiled_shaders::LIGHT_W_TEXTURE_FRAG,
                                   ShaderTypes::FRAGMENT}};

  Shader lightShader2{ShaderProgram{compiled_shaders::LIGHT_W_TEXTURE_VERT,
                                    ShaderTypes::VERTEX},
                      ShaderProgram{compiled_shaders::LIGHT_W_TEXTURE_FRAG,
                                    ShaderTypes::FRAGMENT}};

  Shader planeShader{
      ShaderProgram{compiled_shaders::PLANE_GRAVITY_VERT, ShaderTypes::VERTEX},
      ShaderProgram{compiled_shaders::PLANE_GRAVITY_FRAG,
                    ShaderTypes::FRAGMENT}};

  unsigned int sphereTexture = loadTexture(texturePath);
  unsigned int sphereTexture2 = loadTexture(texture2Path);
  unsigned int sphereTexture3 = loadTexture(texture3Path);
  unsigned int spaceTimeGrid = loadTexture(texture4Path);

  // --- Cache uniform locations for object shader ---
  shader.useShader();
  int modelLoc = glGetUniformLocation(shader.ID, "model");
  int viewLoc = glGetUniformLocation(shader.ID, "view");
  int projLoc = glGetUniformLocation(shader.ID, "projection");
  int objectColorLoc = glGetUniformLocation(shader.ID, "objectColor");
  int lightColorLoc = glGetUniformLocation(shader.ID, "lightColor");
  int lightPosLoc = glGetUniformLocation(shader.ID, "lightPos");
  int lightColor2Loc = glGetUniformLocation(shader.ID, "lightColor2");
  int lightPos2Loc = glGetUniformLocation(shader.ID, "lightPos2");
  int viewPosLoc = glGetUniformLocation(shader.ID, "viewPos");
  int texLoc = glGetUniformLocation(shader.ID, "texture1");

  // --- Cache uniform locations for first light shader ---
  lightShader.useShader();
  int l_modelLoc = glGetUniformLocation(lightShader.ID, "model");
  int l_viewLoc = glGetUniformLocation(lightShader.ID, "view");
  int l_projLoc = glGetUniformLocation(lightShader.ID, "projection");
  int l_lightColorLoc = glGetUniformLocation(lightShader.ID, "lightColor");
  int l_texLoc = glGetUniformLocation(lightShader.ID, "texture1");

  // --- Cache uniform locations for second light shader ---
  lightShader2.useShader();
  int l2_modelLoc = glGetUniformLocation(lightShader2.ID, "model");
  int l2_viewLoc = glGetUniformLocation(lightShader2.ID, "view");
  int l2_projLoc = glGetUniformLocation(lightShader2.ID, "projection");
  int l2_lightColorLoc = glGetUniformLocation(lightShader2.ID, "lightColor");
  int l2_texLoc = glGetUniformLocation(lightShader2.ID, "texture1");

  planeShader.useShader();
  int planeModelLoc = glGetUniformLocation(planeShader.ID, "model");
  int planeViewLoc = glGetUniformLocation(planeShader.ID, "view");
  int planeProjLoc = glGetUniformLocation(planeShader.ID, "projection");
  int planeTexLoc = glGetUniformLocation(planeShader.ID, "texture1");
  int objectUniLocation = glGetUniformLocation(planeShader.ID, "objectPos");
  int massLocation = glGetUniformLocation(planeShader.ID, "mass");

  // Earth Location
  vec3 earthLoc = vec3(1.0f, 1.0f, 1.0f);
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

    // Object shader
    shader.useShader();
    shader.setMat4(modelLoc, glm::translate(mat4(1.0f), earthLoc));
    shader.setMat4(viewLoc, camera.get_view_matrix());
    shader.setMat4(projLoc, camera.get_projection_matrix());
    shader.setVec3(objectColorLoc, vec3(1.f, 1.f, 1.f));
    shader.setVec3(lightColorLoc, vec3(1.f, 0.f, 0.f));
    shader.setVec3(lightPosLoc, lightPos);
    shader.setVec3(lightColor2Loc, vec3(1.f, 1.f, 1.f));
    shader.setVec3(lightPos2Loc, lightPos2);
    shader.setVec3(viewPosLoc, camera.get_position());
    shader.setInt(texLoc, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sphereTexture);
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sphereVerts.size()));

    // === Plane ===
    planeShader.useShader();
    mat4 planeModel = glm::translate(mat4(1.0f), vec3(0.0f, -3.0f, 0.0f));
    planeShader.setMat4(planeModelLoc, planeModel);
    planeShader.setMat4(planeViewLoc, camera.get_view_matrix());
    planeShader.setMat4(planeProjLoc, camera.get_projection_matrix());
    planeShader.setVec3(objectUniLocation, earthLoc);
    planeShader.setFloat(massLocation, 10000.0f);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, spaceTimeGrid);
    planeShader.setInt(planeTexLoc, 3);
    glBindVertexArray(planeVAO);
    glDrawElements(GL_TRIANGLES, planeIndices.size(), GL_UNSIGNED_INT, 0);

    // === First light ===
    lightShader.useShader();
    mat4 lightModel = glm::translate(mat4(1.0f), lightPos);
    lightModel = glm::scale(lightModel, vec3(0.2f));
    lightShader.setMat4(l_modelLoc, lightModel);
    lightShader.setMat4(l_viewLoc, camera.get_view_matrix());
    lightShader.setMat4(l_projLoc, camera.get_projection_matrix());
    lightShader.setVec3(l_lightColorLoc, vec3(1.f, 0.f, 0.f));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sphereTexture2);
    lightShader.setInt(l_texLoc, 1);
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sphereVerts.size()));

    // === Second light ===
    lightShader2.useShader();
    mat4 light2Model = glm::translate(mat4(1.0f), lightPos2);
    light2Model = glm::scale(light2Model, vec3(0.5f));
    lightShader2.setMat4(l2_modelLoc, light2Model);
    lightShader2.setMat4(l2_viewLoc, camera.get_view_matrix());
    lightShader2.setMat4(l2_projLoc, camera.get_projection_matrix());
    lightShader2.setVec3(l2_lightColorLoc, vec3(1.f, 1.f, 1.f));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, sphereTexture3);

    lightShader2.setInt(l2_texLoc, 2);
    glBindVertexArray(sphereVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sphereVerts.size()));

    glfwSwapBuffers(window);
  }
}
