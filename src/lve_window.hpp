#pragma once

#include <string>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lve {
class Window {
public:
  Window(int height, int width, std::string name);
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  ~Window();

  bool shouldClose() { return glfwWindowShouldClose(window_); }
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface_);

private:
  const int height_, width_;
  const std::string name_;
  void initWindow();
  GLFWwindow *window_;
};
} // namespace lve
