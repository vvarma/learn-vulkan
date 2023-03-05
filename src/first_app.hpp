#pragma once

#include "engine_device.hpp"
#include "lve_pipeline.hpp"
#include "lve_window.hpp"

namespace lve {
class FirstApp {
public:
  static constexpr int HEIGHT = 480;
  static constexpr int WIDTH = 640;

  void run();

private:
  Window window_{HEIGHT, WIDTH, "Hello Vulkan!"};
  Device device_{window_};

  Pipeline pipeline_{device_, "shaders/simple_shader.vert.spv",
                     "shaders/simple_shader.frag.spv",
                     Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};
} // namespace lve
