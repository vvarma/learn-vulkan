#include "first_app.hpp"
#include <GLFW/glfw3.h>

namespace lve {
void FirstApp::run() {
  while (!window_.shouldClose()) {
    glfwPollEvents();
  }
}

} // namespace lve
