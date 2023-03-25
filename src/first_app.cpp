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
#include "rubiks.hpp"

namespace lve {

void FirstApp::run() {
  SimpleRenderSystem simple_render_system(device_,
                                          renderer_.getSwapChainRenderPass());
  Camera camera{};

  auto viewerObject = GameObject::createGameObject();
  KeyboardMovementController cameraController{};
  auto currentTime = std::chrono::high_resolution_clock::now();

  RubiksCube rubiks{};

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

void FirstApp::loadGameObjects() {
  RubiksCube rubiks{};
  auto builder = rubiks.getModel();
  auto model = std::make_shared<Model>(device_, rubiks.getModel());
  auto cube = GameObject::createGameObject();
  cube.model = model;
  cube.transform.translation = {.0f, .0f, 2.5f};
  cube.transform.scale = {.5f, .5f, .5f};
  game_objects_.push_back(std::move(cube));
}

FirstApp::FirstApp() { loadGameObjects(); }

FirstApp::~FirstApp() {}

} // namespace lve
