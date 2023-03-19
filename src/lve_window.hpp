#pragma once

#include <cstdint>
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
  VkExtent2D getExtent() {
    return {static_cast<uint32_t>(width_), static_cast<uint32_t>(height_)};
  }
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface_);
  bool wasWindowResized();
  void resetWindowResizedFlag();
  GLFWwindow *getGLFWwindow() const { return window_; }

private:
  static void framebufferResizeCallack(GLFWwindow *window, int width,
                                       int height);
  int height_, width_;
  bool framebuffer_resized_;
  const std::string name_;
  void initWindow();
  GLFWwindow *window_;
};
} // namespace lve
