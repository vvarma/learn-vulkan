#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "device.hpp"
#include "lve_window.hpp"
#include "swap_chain.hpp"

namespace lve {
class Renderer {
public:
  Renderer(Window &window, Device &device);
  ~Renderer();
  Renderer(const Renderer &) = delete;
  Renderer &operator=(const Renderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const {
    return swap_chain_->getRenderPass();
  }
  float getAspectRatio() const { return swap_chain_->extentAspectRatio(); }
  bool isFrameInProgress() const { return is_frame_started_; }
  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(is_frame_started_ &&
           "Cannot get command buffer when frame not in progress");
    return command_buffer_[current_frame_idx_];
  }

  int getFrameIndex() const {
    assert(is_frame_started_ &&
           "Cannot getFrameIndex when frame not in progress");
    return current_frame_idx_;
  }

  VkCommandBuffer beginFrame();
  void endFrame();

  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void drawFame();
  void recreateSwapChain();
  Window &window_;
  Device &device_;
  std::unique_ptr<SwapChain> swap_chain_;
  std::vector<VkCommandBuffer> command_buffer_;

  uint32_t current_image_idx_;
  int current_frame_idx_{0};
  bool is_frame_started_{false};
};
} // namespace lve
