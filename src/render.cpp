#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include "renderer.hpp"
#include "swap_chain.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {

Renderer::Renderer(Window &window, Device &device)
    : window_(window), device_(device) {
  recreateSwapChain();
  createCommandBuffers();
}

Renderer::~Renderer() { freeCommandBuffers(); }

void Renderer::createCommandBuffers() {
  command_buffer_.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device_.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(command_buffer_.size());
  if (vkAllocateCommandBuffers(device_.device(), &allocInfo,
                               command_buffer_.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers");
  }
}
void Renderer::freeCommandBuffers() {
  vkFreeCommandBuffers(device_.device(), device_.getCommandPool(),
                       static_cast<uint32_t>(command_buffer_.size()),
                       command_buffer_.data());
  command_buffer_.clear();
}
void Renderer::recreateSwapChain() {
  auto extent = window_.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = window_.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(device_.device());
  if (swap_chain_ == nullptr)
    swap_chain_ = std::make_unique<SwapChain>(device_, extent);

  else {
    std::shared_ptr<SwapChain> oldSwapChain = std::move(swap_chain_);
    swap_chain_ = std::make_unique<SwapChain>(device_, extent, oldSwapChain);
    if (!oldSwapChain->compareSwapFormats(*swap_chain_.get())) {
      throw std::runtime_error("Swap chain image(or depth) format has changed");
    }
  }
  // todo check if render pass is compatible then skip
  // createPipline();
}

VkCommandBuffer Renderer::beginFrame() {
  assert(!is_frame_started_ &&
         "Can't call beginFrame while already in progress");
  auto result = swap_chain_->acquireNextImage(&current_image_idx_);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return nullptr;
  }
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image");
  }
  is_frame_started_ = true;
  auto commandBuffer = getCurrentCommandBuffer();
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer");
  }
  return commandBuffer;
}
void Renderer::endFrame() {
  assert(is_frame_started_ &&
         "can't call endframe while frame is not in progress");
  auto commandBuffer = getCurrentCommandBuffer();
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer");
  }

  auto result =
      swap_chain_->submitCommandBuffers(&commandBuffer, &current_image_idx_);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      window_.wasWindowResized()) {
    window_.resetWindowResizedFlag();
    recreateSwapChain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image");
  }
  is_frame_started_ = false;
  current_frame_idx_ =
      (current_frame_idx_ + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(is_frame_started_ &&
         "can't call beginSwapChainRenderPass while frame is not in progress");
  assert(commandBuffer == getCurrentCommandBuffer() &&
         "Can't beginSwapChainRenderPass on command buffer from a different "
         "frame");

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = swap_chain_->getRenderPass();
  renderPassInfo.framebuffer = swap_chain_->getFrameBuffer(current_image_idx_);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swap_chain_->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swap_chain_->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(swap_chain_->getSwapChainExtent().height);
  viewport.maxDepth = 1.0f;
  viewport.minDepth = 0.0f;
  VkRect2D scissor{{0, 0}, swap_chain_->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
  assert(is_frame_started_ &&
         "can't call endSwapChainRenderPass while frame is not in progress");
  assert(
      commandBuffer == getCurrentCommandBuffer() &&
      "Can't endSwapChainRenderPass on command buffer from a different frame");

  vkCmdEndRenderPass(commandBuffer);
}

} // namespace lve
