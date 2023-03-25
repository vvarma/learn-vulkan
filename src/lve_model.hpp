#pragma once
#include "device.hpp"

#include <cstdint>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace lve {
class Model {
public:
  struct Vertex {
    glm::vec3 position{};
    glm::vec3 color{};
    bool operator==(const Vertex &other) const;
    static std::vector<VkVertexInputBindingDescription>
    getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescriptions();
  };
  struct Builder {
    std::vector<Vertex> vertices{};
    std::vector<uint32_t> indices{};
  };
  Model(Device &device, const Model::Builder &builder);
  ~Model();
  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;
  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  void createVertexBuffers(const std::vector<Vertex> &vertices);
  void createIndexBuffers(const std::vector<uint32_t> &indices);
  Device &device_;
  VkBuffer vertex_buffer_;
  VkDeviceMemory vertex_buffer_memory_;
  uint32_t vertex_count_;

  bool has_index_buffer = false;

  VkBuffer index_buffer_;
  VkDeviceMemory index_buffer_memory_;
  uint32_t index_count_;
};

} // namespace lve

namespace std {
template <> struct hash<lve::Model::Vertex> {
  size_t operator()(lve::Model::Vertex const &vertex) const {
    size_t seed = 0;
    hash<glm::vec3> hasher{};
    glm::detail::hash_combine(seed, hasher(vertex.position));
    glm::detail::hash_combine(seed, hasher(vertex.color));
    return seed;
  }
};
} // namespace std
