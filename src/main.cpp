#include "first_app.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>
int main(int argc, char *argv[]) {
  lve::FirstApp app;
  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
