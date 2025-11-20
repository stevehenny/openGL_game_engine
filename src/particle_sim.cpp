#include "Camera.h"
#include "MeshPlane.h"
#include "Particle.h"
#include "top_level_include.h"
#include "window_functions.h"
#include <iostream>
int main(int argc, char *argv[]) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(1920, 1080, "PARTICLE_SIM", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  Camera camera{vec3(0.0f, 0.0f, 3.0f),
                vec3(0.f, 0.f, -1.f),
                vec3(0.f, 1.f, 0.f),
                (float)1920,
                (float)1080,
                0.1f,
                0.1f,
                100.f,
                10.0f};
  glfwSetWindowUserPointer(window, &camera);
  glfwSetCursorPosCallback(window, Camera::mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  MeshPlane mesh{10.0, 10.0, 20u, 20u, 1.0f, 1.0f};
  Particles particles(2u);

  mesh.getShader().useShader();
  int planeModelLoc = glGetUniformLocation(mesh.getShader().ID, "model");
  int planeViewLoc = glGetUniformLocation(mesh.getShader().ID, "view");
  int planeProjLoc = glGetUniformLocation(mesh.getShader().ID, "projection");

  // glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)) {

    camera.update_camera_delta_time(static_cast<float>(glfwGetTime()));
    camera.move_camera(window);
    process_window_input(window);
    glfwPollEvents();
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT);

    // set mesh
    mesh.getShader().useShader();
    mat4 planeModel = glm::translate(mat4(1.0f), mesh.getLocation());
    mesh.getShader().setMat4(planeModelLoc, planeModel);
    mesh.getShader().setMat4(planeViewLoc, camera.get_view_matrix());
    mesh.getShader().setMat4(planeProjLoc, camera.get_projection_matrix());
    mesh.draw();
    glfwSwapBuffers(window);
  }

  return 0;
}
