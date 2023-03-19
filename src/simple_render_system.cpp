#include "simple_render_system.hpp"
#include "device.hpp"
#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include "swap_chain.hpp"
#include <GLFW/glfw3.h>
#include <array>
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
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vulkan/vulkan_core.h>

namespace lve {

struct SimplePushConstantsData {
  glm::mat4 transform{1.0f};
  alignas(16) glm::vec3 color;
};

SimpleRenderSystem::SimpleRenderSystem(Device &device, VkRenderPass renderPass)
    : device_(device) {
  createPipleineLayout();
  createPipline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
  vkDestroyPipelineLayout(device_.device(), pipeline_layout_, nullptr);
}
void SimpleRenderSystem::createPipleineLayout() {

  VkPushConstantRange pushConstantRange{};
  pushConstantRange.size = sizeof(SimplePushConstantsData);
  pushConstantRange.offset = 0;
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
  pipelineLayoutCreateInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutCreateInfo.setLayoutCount = 0;
  pipelineLayoutCreateInfo.pSetLayouts = nullptr;
  pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
  pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutCreateInfo,
                             nullptr, &pipeline_layout_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout");
  }
}
void SimpleRenderSystem::createPipline(VkRenderPass renderPass) {
  assert(pipeline_layout_ != nullptr &&
         "Cannot create pipeline before pipeline layout");
  PipelineConfigInfo pipelineConfig{};
  Pipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderPass;
  pipelineConfig.pipelineLayout = pipeline_layout_;
  pipeline_ = std::make_unique<Pipeline>(
      device_, "shaders/simple_shader.vert.spv",
      "shaders/simple_shader.frag.spv", pipelineConfig);
}

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer,
                                           std::vector<GameObject> &gameObjects,
                                           const Camera &camera) {
  pipeline_->bind(commandBuffer);
  auto projectionView = camera.getProjection() * camera.getView();
  for (auto &obj : gameObjects) {
    SimplePushConstantsData push{};
    push.color = obj.color;
    push.transform = projectionView * obj.transform.mat4();
    vkCmdPushConstants(commandBuffer, pipeline_layout_,
                       VK_SHADER_STAGE_FRAGMENT_BIT |
                           VK_SHADER_STAGE_VERTEX_BIT,
                       0, sizeof(SimplePushConstantsData), &push);
    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}

} // namespace lve
