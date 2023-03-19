#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "camera.hpp"
#include "device.hpp"
#include "lve_game_object.hpp"
#include "lve_pipeline.hpp"

namespace lve {
class SimpleRenderSystem {
public:
  SimpleRenderSystem(Device &device, VkRenderPass renderPass);
  ~SimpleRenderSystem();
  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;
  void renderGameObjects(VkCommandBuffer commandBuffer,
                         std::vector<GameObject> &gameObjects,
                         const Camera &camera);

private:
  void createPipleineLayout();
  void createPipline(VkRenderPass renderPass);
  Device &device_;
  std::unique_ptr<Pipeline> pipeline_;
  VkPipelineLayout pipeline_layout_;
};
} // namespace lve
