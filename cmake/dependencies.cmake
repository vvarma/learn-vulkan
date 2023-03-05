if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(
    DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)
configure_file(${CMAKE_SOURCE_DIR}/conanfile.txt
               ${CMAKE_BINARY_DIR}/conanfile.txt COPYONLY)

set(CONAN_HOST_PROFILE
    "default"
    CACHE STRING "Conan profile of target machine")
set(CONAN_BUILD_PROFILE
    "default"
    CACHE STRING "Conan profile of target machine")

conan_cmake_install(
  PATH_OR_REFERENCE
  .
  BUILD
  missing
  PROFILE_HOST
  ${CONAN_HOST_PROFILE}
  PROFILE_BUILD
  ${CONAN_BUILD_PROFILE})

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
#conan_basic_setup(TARGETS)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(shaderc REQUIRED)
find_package(vulkan-validationlayers REQUIRED)
