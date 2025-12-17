#include "Spheres.h"
#include "Camera.h"
#include "Common.h"
#include "Cube.h"
#include "MeshPlane.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ShaderPrograms.h"
#include "Texture.h"
#include "external/stb/stb_easy_font.h"
#include "window_functions.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#include <stb_image.h>
#include <vector>

using glm::mat4;
using glm::vec3;

static constexpr unsigned int SCR_WIDTH = 1920;
static constexpr unsigned int SCR_HEIGHT = 1080;

int main(int argc, char *argv[]) {
  std::cerr << "Usage: ./lighting <texture1> <texture2> <texture3> "
               "<texture4> <font>\n";
  if (argc < 4) {
    return -1;
  }

  char *texturePath = argv[1];
  char *texture2Path = argv[2];
  char *texture3Path = argv[3];

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
  glfwSwapInterval(0);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  Camera camera{vec3(0.0f, 0.0f, 3.0f),
                vec3(0.f, 0.f, -1.f),
                vec3(0.f, 1.f, 0.f),
                (float)SCR_WIDTH,
                (float)SCR_HEIGHT,
                0.1f,
                0.1f,
                100.f,
                10.0f};
  glfwSetWindowUserPointer(window, &camera);
  glfwSetCursorPosCallback(window, Camera::mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Spheres spheres{1.0f, 36, 18};

  MeshPlane mesh{300.0f, 300.0f, 300u, 300u, 0.0f, 0.98f};

  Shader lightShader{ShaderProgram{compiled_shaders::LIGHT_W_TEXTURE_VERT,
                                   ShaderTypes::VERTEX},
                     ShaderProgram{compiled_shaders::LIGHT_W_TEXTURE_FRAG,
                                   ShaderTypes::FRAGMENT}};
  Shader lightShader2{ShaderProgram{compiled_shaders::LIGHT_W_TEXTURE_VERT,
                                    ShaderTypes::VERTEX},
                      ShaderProgram{compiled_shaders::LIGHT_W_TEXTURE_FRAG,
                                    ShaderTypes::FRAGMENT}};

  // update bindless textures
  GLuint texture1, texture2, texture3;
  std::vector<const char *> textures = {texturePath, texture2Path,
                                        texture3Path};
  GLuint textureArray = loadTextureArray(textures);
  spheres.getShader().useShader();
  // spheres.updateTextures(textures);
  int modelLoc = glGetUniformLocation(spheres.getShader().ID, "model");
  int viewLoc = glGetUniformLocation(spheres.getShader().ID, "view");
  int projLoc = glGetUniformLocation(spheres.getShader().ID, "projection");
  int objectColorLoc =
      glGetUniformLocation(spheres.getShader().ID, "objectColor");
  int lightColorLoc =
      glGetUniformLocation(spheres.getShader().ID, "lightColor");
  int lightPosLoc = glGetUniformLocation(spheres.getShader().ID, "lightPos");
  int lightColor2Loc =
      glGetUniformLocation(spheres.getShader().ID, "lightColor2");
  int lightPos2Loc = glGetUniformLocation(spheres.getShader().ID, "lightPos2");
  int viewPosLoc = glGetUniformLocation(spheres.getShader().ID, "viewPos");
  int texLoc = glGetUniformLocation(spheres.getShader().ID, "texture1");
  int texturesLoc = glGetUniformLocation(spheres.getShader().ID, "textures");
  int numSpheresLoc =
      glGetUniformLocation(spheres.getShader().ID, "numSpheres");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
  spheres.getShader().setInt(texturesLoc, 0);

  mesh.getShader().useShader();
  int planeModelLoc = glGetUniformLocation(mesh.getShader().ID, "model");
  int planeViewLoc = glGetUniformLocation(mesh.getShader().ID, "view");
  int planeProjLoc = glGetUniformLocation(mesh.getShader().ID, "projection");
  int planeTexLoc = glGetUniformLocation(mesh.getShader().ID, "texture1");
  int objectUniLocation =
      glGetUniformLocation(mesh.getShader().ID, "objectPos");
  int massLocation = glGetUniformLocation(mesh.getShader().ID, "mass");
  int forceWhiteLoc = glGetUniformLocation(mesh.getShader().ID, "forceWhite");
  int numObjectsLoc = glGetUniformLocation(mesh.getShader().ID, "numObjects");

  glEnable(GL_DEPTH_TEST);
  double lastTime = glfwGetTime();
  int nbFrames = 0;

  vector<Sphere> sphereData;
  sphereData.push_back(Sphere{.objectColor = vec4(1.0f),
                              .position = vec4(20.0f, 1.0f, -20.0f, 1.0f),
                              .force = vec4(0.0f),
                              .velocity = vec4(-1.0f, 0.0f, -1.0f, 1.0f),
                              .Norm = vec4(1.0f),
                              .lightColor = vec4(1.0f, 0.0f, 0.0f, 0.0f),
                              .specStrength = 0.5f,
                              .ambStrength = 0.5f,
                              .radius = 1.0f,
                              .mass = 1e13,
                              .textureInd = 1,
                              .isLightSource = true});

  sphereData.push_back(Sphere{.objectColor = vec4(1.0f),
                              .position = vec4(-20.0f, 1.0f, 20.0f, 1.0f),
                              .force = vec4(0.0f),
                              .velocity = vec4(1.0f, -0.0f, 1.0f, 1.0f),
                              .Norm = vec4(1.0f),
                              .lightColor = vec4(1.0f),
                              .specStrength = 0.5f,
                              .ambStrength = 0.5f,
                              .radius = 1.0f,
                              .mass = 1e13,
                              .textureInd = 0,
                              .isLightSource = false});

  sphereData.push_back(Sphere{.objectColor = vec4(1.0f),
                              .position = vec4(40.0f, 1.0f, -40.0f, 1.0f),
                              .force = vec4(0.0f),
                              .velocity = vec4(1.0f, -0.0f, 1.0f, 1.0f),
                              .Norm = vec4(1.0f),
                              .lightColor = vec4(1.0f),
                              .specStrength = 0.5f,
                              .ambStrength = 0.5f,
                              .radius = 1.0f,
                              .mass = 1e13,
                              .textureInd = 0,
                              .isLightSource = false});

  std::mt19937 uniformEngine(std::random_device{}());

  std::uniform_real_distribution<float> velocity(-1, 1);
  std::uniform_real_distribution<float> position(-100, 100);
  // for (int i = 0; i < 1000; ++i) {
  //   sphereData.push_back(Sphere{
  //       vec4(1.0f),
  //       vec4(position(uniformEngine), 1.0f, position(uniformEngine), 1.0f),
  //       vec4(0.0f), vec4(velocity(uniformEngine)), 1.0f, 1e13, 0});
  // }

  spheres.updateSBBO(sphereData);
  float dt = 0.0001f;
  // mesh.updateSBBO(sphereData);
  mesh.setSphereSBBO(spheres.getSphereSBBO(), sphereData.size());
  // spheres.getShader().setUniformTextures(texturesLoc, textures);
  int curCount{};
  int RefreshCount = 10;
  while (!glfwWindowShouldClose(window)) {
    ++curCount;
    glfwPollEvents();
    //*******CAMERA********//
    if (curCount >= RefreshCount) {
    }
    float time = glfwGetTime();
    camera.update_camera_delta_time(time);
    camera.move_camera(window);
    //*******CAMERA********//

    //*********FRAMERATE*********//
    nbFrames++;
    double fps = 0.0;
    if (time - lastTime >= 1.0) {
      fps = static_cast<double>(nbFrames) / (time - lastTime);
      nbFrames = 0;
      lastTime = time;
      std::cout << "FPS: " << fps << '\n';
      std::cout << "Texture array loc: " << texturesLoc << '\n';
    }
    //*********FRAMERATE*********//

    //*********SPHERE COMPUTE AND SET MESH SBBO*************//
    spheres.runComputeShader(dt);
    mesh.updateSBBO_from_GPU();
    //*********SPHERE COMPUTE AND SET MESH SBBO*************//

    //********CLEAR COLOR AND PREPARE NEXT FRAME************//
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //********CLEAR COLOR AND PREPARE NEXT FRAME************//

    //***********DRAW SPHERES********************//
    spheres.getShader().useShader();
    // spheres.getShader().setMat4(modelLoc, model);
    spheres.getShader().setMat4(viewLoc, camera.get_view_matrix());
    spheres.getShader().setMat4(projLoc, camera.get_projection_matrix());
    spheres.getShader().setVec3(objectColorLoc, vec3(1.f, 1.f, 1.f));
    spheres.getShader().setVec3(lightColorLoc, vec3(1.f, 0.f, 0.f));
    // spheres.getShader().setVec3(lightPosLoc, objects[1].position);
    spheres.getShader().setVec3(lightColor2Loc, vec3(1.f, 1.f, 1.f));
    // spheres.getShader().setVec3(lightPos2Loc, objects[2].position);
    spheres.getShader().setVec3(viewPosLoc, camera.get_position());
    spheres.getShader().setInt(texLoc, 0);
    spheres.getShader().setInt(numSpheresLoc, spheres.getSpheres().size());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    spheres.draw();
    //***********DRAW SPHERES********************//

    //******DRAW MESH PLANE************//
    mesh.getShader().useShader();
    mat4 planeModel = glm::translate(mat4(1.0f), mesh.getLocation());
    mesh.getShader().setMat4(planeModelLoc, planeModel);
    mesh.getShader().setMat4(planeViewLoc, camera.get_view_matrix());
    mesh.getShader().setMat4(planeProjLoc, camera.get_projection_matrix());
    mesh.getShader().setInt(numObjectsLoc, static_cast<int>(sphereData.size()));
    mesh.draw();
    //******DRAW MESH PLANE************//

    glfwSwapBuffers(window);
  }
}
