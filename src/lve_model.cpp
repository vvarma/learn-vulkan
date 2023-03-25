#include "lve_model.hpp"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <vulkan/vulkan_core.h>

namespace lve {

Model::Model(Device &device, const Model::Builder &builder) : device_(device) {
  createVertexBuffers(builder.vertices);
  createIndexBuffers(builder.indices);
}
Model::~Model() {
  vkDestroyBuffer(device_.device(), vertex_buffer_, nullptr);
  vkFreeMemory(device_.device(), vertex_buffer_memory_, nullptr);
  if (has_index_buffer) {
    vkDestroyBuffer(device_.device(), index_buffer_, nullptr);
    vkFreeMemory(device_.device(), index_buffer_memory_, nullptr);
  }
}

void Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
  vertex_count_ = static_cast<uint32_t>(vertices.size());
  assert(vertex_count_ >= 3 && "Vertex count must be atleast 3");
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertex_count_;
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  device_.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       stagingBuffer, stagingBufferMemory);
  void *data;
  vkMapMemory(device_.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(device_.device(), stagingBufferMemory);

  device_.createBuffer(bufferSize,
                       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                           VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertex_buffer_,
                       vertex_buffer_memory_);
  device_.copyBuffer(stagingBuffer, vertex_buffer_, bufferSize);
  vkDestroyBuffer(device_.device(), stagingBuffer, nullptr);
  vkFreeMemory(device_.device(), stagingBufferMemory, nullptr);
}

void Model::createIndexBuffers(const std::vector<uint32_t> &indices) {
  index_count_ = static_cast<uint32_t>(indices.size());
  has_index_buffer = index_count_ > 0;
  if (!has_index_buffer)
    return;
  VkDeviceSize bufferSize = sizeof(indices[0]) * index_count_;
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  device_.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       stagingBuffer, stagingBufferMemory);
  void *data;
  vkMapMemory(device_.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(device_.device(), stagingBufferMemory);

  device_.createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, index_buffer_, index_buffer_memory_);
  device_.copyBuffer(stagingBuffer, index_buffer_, bufferSize);
  vkDestroyBuffer(device_.device(), stagingBuffer, nullptr);
  vkFreeMemory(device_.device(), stagingBufferMemory, nullptr);
}
void Model::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {vertex_buffer_};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
  if (has_index_buffer) {
    vkCmdBindIndexBuffer(commandBuffer, index_buffer_, 0, VK_INDEX_TYPE_UINT32);
  }
}
void Model::draw(VkCommandBuffer commandBuffer) {
  if (has_index_buffer)
    vkCmdDrawIndexed(commandBuffer, index_count_, 1, 0, 0, 0);
  else
    vkCmdDraw(commandBuffer, vertex_count_, 1, 0, 0);
}

bool Model::Vertex::operator==(const Model::Vertex &other) const {
  return position == other.position && color == other.color;
}

std::vector<VkVertexInputBindingDescription>
Model::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}
std::vector<VkVertexInputAttributeDescription>
Model::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, position);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);
  return attributeDescriptions;
}
} // namespace lve
