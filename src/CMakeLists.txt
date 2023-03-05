file(GLOB SRCS *.cpp *.hpp)
add_executable(demo ${SRCS})

target_link_libraries(demo glm::glm glfw Vulkan::Vulkan
                      vulkan-validationlayers::vulkan-validationlayers)
