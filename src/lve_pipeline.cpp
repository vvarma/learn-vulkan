#include "lve_pipeline.hpp"
#include <_types/_uint32_t.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
namespace lve {

std::vector<char> Pipeline::readFile(const std::string &filepath) {
  std::ifstream file(filepath, std::ios::ate | std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open file:" + filepath);
  }

  size_t size = static_cast<size_t>(file.tellg());
  std::vector<char> buffer(size);
  file.seekg(0);
  file.read(buffer.data(), size);
  file.close();
  return buffer;
}

Pipeline::Pipeline(Device &device, const std::string &vert_file,
                   const std::string &frag_file,
                   const PipelineConfigInfo &configInfo)
    : device_(device) {
  createGraphicsPipeline(vert_file, frag_file, configInfo);
}
Pipeline::~Pipeline() {}

void Pipeline::createGraphicsPipeline(const std::string &vert_file,
                                      const std::string &frag_file,
                                      const PipelineConfigInfo &configInfo) {
  auto vertCode = readFile(vert_file);
  auto fragCode = readFile(frag_file);
  std::cout << "Vertex shader code size:" << vertCode.size() << '\n';
  std::cout << "Frag shader code size:" << fragCode.size() << '\n';
}

void Pipeline::createShaderModule(std::vector<char> &code,
                                  VkShaderModule *shaderModule) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
  if (vkCreateShaderModule(device_.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to created shader module");
  }
}

PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width,
                                                       uint32_t height) {
  PipelineConfigInfo configInfo{};
  return configInfo;
}

} // namespace lve
