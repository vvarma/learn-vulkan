#include "lve_window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace lve {
Window::Window(int height, int width, std::string name)
    : height_(height), width_(width), name_(name) {
  initWindow();
}

void Window::initWindow() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  window_ = glfwCreateWindow(width_, height_, name_.c_str(), nullptr, nullptr);
}
Window::~Window() {
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface_) {
  if (glfwCreateWindowSurface(instance, window_, nullptr, surface_) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface");
  }
}

} // namespace lve
