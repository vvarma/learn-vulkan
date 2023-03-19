#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "device.hpp"
#include "lve_game_object.hpp"
#include "lve_window.hpp"
#include "renderer.hpp"

namespace lve {
class FirstApp {
public:
  static constexpr int HEIGHT = 480;
  static constexpr int WIDTH = 640;

  void run();
  FirstApp();
  ~FirstApp();
  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

private:
  void loadGameObjects();
  void sierpinski(uint32_t depth, const glm::vec2 &top, const glm::vec2 &right,
                  const glm::vec2 &left, std::array<glm::vec3, 3> &colors,
                  std::vector<Model::Vertex> &vertices);
  Window window_{HEIGHT, WIDTH, "Hello Vulkan!"};
  Device device_{window_};
  std::vector<GameObject> game_objects_;
  Renderer renderer_{window_, device_};
};
} // namespace lve
