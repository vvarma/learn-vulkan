#pragma once
#include "engine_device.hpp"
#include <_types/_uint32_t.h>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>
namespace lve {
struct PipelineConfigInfo {};
class Pipeline {
public:
  Pipeline(Device &device, const std::string &vert_file,
           const std::string &frag_file, const PipelineConfigInfo &configInfo);
  ~Pipeline();
  Pipeline(const Pipeline &) = delete;
  void operator=(const Pipeline &) = delete;

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);

private:
  static std::vector<char> readFile(const std::string &filepath);
  void createGraphicsPipeline(const std::string &vert_file,
                              const std::string &frag_file,
                              const PipelineConfigInfo &configInfo);

  void createShaderModule(std::vector<char> &code,
                          VkShaderModule *shaderModule);
  Device &device_;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};

} // namespace lve
