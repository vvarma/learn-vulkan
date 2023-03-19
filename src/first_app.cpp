#include "first_app.hpp"
#include "lve_game_object.hpp"
#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include "swap_chain.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <chrono>
#include <cstdint>
#include <glm/fwd.hpp>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <vector>

#include <vulkan/vulkan_core.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "simple_render_system.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vulkan/vulkan_core.h>

#include "camera.hpp"
#include "keyboard_movement_controller.hpp"

namespace lve {

void FirstApp::run() {
  SimpleRenderSystem simple_render_system(device_,
                                          renderer_.getSwapChainRenderPass());
  Camera camera{};

  auto viewerObject = GameObject::createGameObject();
  KeyboardMovementController cameraController{};
  auto currentTime = std::chrono::high_resolution_clock::now();

  while (!window_.shouldClose()) {
    glfwPollEvents();
    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime -
                                                                   currentTime)
            .count();
    currentTime = newTime;
    cameraController.moveInPlaneXZ(window_.getGLFWwindow(), frameTime,
                                   viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation,
                      viewerObject.transform.rotation);
    float aspect = renderer_.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
    if (auto commandBuffer = renderer_.beginFrame()) {
      renderer_.beginSwapChainRenderPass(commandBuffer);
      simple_render_system.renderGameObjects(commandBuffer, game_objects_,
                                             camera);
      renderer_.endSwapChainRenderPass(commandBuffer);
      renderer_.endFrame();
    }
  }
  vkDeviceWaitIdle(device_.device());
}
std::unique_ptr<Model> createCubeModel(Device &device, glm::vec3 offset) {
  Model::Builder modelBuilder{};
  modelBuilder.vertices = {

      // left face (white)
      {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
      {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

      // top face (orange, remember y axis points down)
      {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
      {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
      {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
      {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
      {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
  };
  for (auto &v : modelBuilder.vertices) {
    v.position += offset;
  }
  modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,
                          8,  9,  10, 8,  11, 9,  12, 13, 14, 12, 15, 13,
                          16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
  return std::make_unique<Model>(device, modelBuilder);
}

void FirstApp::loadGameObjects() {
  std::shared_ptr<Model> model = createCubeModel(device_, {0.f, .0f, .0f});
  auto cube = GameObject::createGameObject();
  cube.model = model;
  cube.transform.translation = {.0f, .0f, 2.5f};
  cube.transform.scale = {.5f, .5f, .5f};
  game_objects_.push_back(std::move(cube));
}

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

} // namespace lve
