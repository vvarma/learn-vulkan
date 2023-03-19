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
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  window_ = glfwCreateWindow(width_, height_, name_.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(window_, this);
  glfwSetFramebufferSizeCallback(window_, Window::framebufferResizeCallack);
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

bool Window::wasWindowResized() { return framebuffer_resized_; }
void Window::resetWindowResizedFlag() { framebuffer_resized_ = false; }

void Window::framebufferResizeCallack(GLFWwindow *window, int width,
                                      int height) {
  auto lve_window =
      reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  lve_window->width_ = width;
  lve_window->height_ = height;
  lve_window->framebuffer_resized_ = true;
}

} // namespace lve
