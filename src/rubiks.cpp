#include "rubiks.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace lve {

glm::vec3 resolveColor(Color color) {
  switch (color) {
  case White:
    return {1.f, 1.f, 1.f};
  case Blue:
    return {0.f, 0.f, 1.f};
  case Red:
    return {1.f, 0.f, 0.f};
  case Green:
    return {0.f, 1.f, 0.f};
  case Yellow:
    return {1.f, 1.f, 0.f};
  case Black:
    return {0.f, 0.f, 0.f};
  case Orange:
    return {.9f, 0.5f, 0.0f};
  default:
    return {0.2f, 0.2f, 0.2f};
  }
}

uint64_t makeUniformFace(Color color) {
  uint64_t ret = 0;
  for (int i = 0; i < 8; ++i) {
    ret = (ret << 4) | color;
  }
  return ret;
}

std::array<uint64_t, 6> makeRubiks() {
  return {
      makeUniformFace(faceColors[0]), makeUniformFace(faceColors[1]),
      makeUniformFace(faceColors[2]), makeUniformFace(faceColors[3]),
      makeUniformFace(faceColors[4]), makeUniformFace(faceColors[5]),
  };
}

RubiksCube::RubiksCube() : data(makeRubiks()) {}

std::vector<uint32_t>
insertIfNotExist(std::vector<Model::Vertex> &vertices,
                 std::unordered_map<Model::Vertex, uint32_t> &uniqueVertices,
                 std::vector<Model::Vertex> toInsert) {
  std::vector<uint32_t> ret{};
  for (auto &vertex : toInsert) {
    if (uniqueVertices.count(vertex) == 0) {
      uniqueVertices[vertex] = uniqueVertices.size();
      vertices.push_back(vertex);
    }
    ret.push_back(uniqueVertices[vertex]);
  }
  return ret;
}

void insertCube(std::vector<Model::Vertex> &vertices,
                std::unordered_map<Model::Vertex, uint32_t> &uniqueVertices,
                std::vector<uint32_t> &indices, glm::vec3 nearLeft, float step,
                std::array<Color, 6> colors) {
  // front
  auto points = insertIfNotExist(
      vertices, uniqueVertices,
      {
          {nearLeft, resolveColor(colors[0])},
          {nearLeft + glm::vec3{step, 0.f, 0.f}, resolveColor(colors[0])},
          {nearLeft + glm::vec3{0.f, step, 0.f}, resolveColor(colors[0])},
          {nearLeft + glm::vec3{step, step, 0.f}, resolveColor(colors[0])},
      });
  indices.insert(indices.end(), {points[0], points[1], points[2], points[1],
                                 points[3], points[2]});
  // left
  points = insertIfNotExist(
      vertices, uniqueVertices,
      {
          {nearLeft, resolveColor(colors[1])},
          {nearLeft + glm::vec3{0.f, 0.f, step}, resolveColor(colors[1])},
          {nearLeft + glm::vec3{0.f, step, 0.f}, resolveColor(colors[1])},
          {nearLeft + glm::vec3{0.f, step, step}, resolveColor(colors[1])},
      });
  indices.insert(indices.end(), {points[0], points[1], points[2], points[1],
                                 points[3], points[2]});
  // top
  points = insertIfNotExist(
      vertices, uniqueVertices,
      {
          {nearLeft, resolveColor(colors[2])},
          {nearLeft + glm::vec3{0.f, 0.f, step}, resolveColor(colors[2])},
          {nearLeft + glm::vec3{step, 0.f, 0.f}, resolveColor(colors[2])},
          {nearLeft + glm::vec3{step, 0.f, step}, resolveColor(colors[2])},
      });
  indices.insert(indices.end(), {points[0], points[1], points[2], points[1],
                                 points[3], points[2]});
  // right
  points = insertIfNotExist(
      vertices, uniqueVertices,
      {
          {nearLeft + glm::vec3{step, 0.f, 0.f}, resolveColor(colors[3])},
          {nearLeft + glm::vec3{step, 0.f, step}, resolveColor(colors[3])},
          {nearLeft + glm::vec3{step, step, 0.f}, resolveColor(colors[3])},
          {nearLeft + glm::vec3{step, step, step}, resolveColor(colors[3])},
      });
  indices.insert(indices.end(), {points[0], points[1], points[2], points[1],
                                 points[3], points[2]});
  // bottom
  points = insertIfNotExist(
      vertices, uniqueVertices,
      {
          {nearLeft + glm::vec3{0.f, step, 0.f}, resolveColor(colors[4])},
          {nearLeft + glm::vec3{0.f, step, step}, resolveColor(colors[4])},
          {nearLeft + glm::vec3{step, step, 0.f}, resolveColor(colors[4])},
          {nearLeft + glm::vec3{step, step, step}, resolveColor(colors[4])},
      });
  indices.insert(indices.end(), {points[0], points[1], points[2], points[1],
                                 points[3], points[2]});
  // back
  points = insertIfNotExist(
      vertices, uniqueVertices,
      {
          {nearLeft + glm::vec3{0.f, 0.f, step}, resolveColor(colors[5])},
          {nearLeft + glm::vec3{step, 0.f, step}, resolveColor(colors[5])},
          {nearLeft + glm::vec3{0.f, step, step}, resolveColor(colors[5])},
          {nearLeft + glm::vec3{step, step, step}, resolveColor(colors[5])},
      });
  indices.insert(indices.end(), {points[0], points[1], points[2], points[1],
                                 points[3], points[2]});
}

Model::Builder RubiksCube::getModel() const {
  auto builder = Model::Builder{};
  float min = -0.38f, sep = 0.02f, step = 0.3f;
  std::unordered_map<Model::Vertex, uint32_t> uniqueVertices{};
  for (int x = 0; x < 3; ++x) {
    for (int y = 0; y < 3; ++y) {
      for (int z = 0; z < 3; ++z) {
        float x_loc = min + (sep + step) * x;
        float y_loc = min + (sep + step) * y;
        float z_loc = min + (sep + step) * z;

        insertCube(builder.vertices, uniqueVertices, builder.indices,
                   {x_loc, y_loc, z_loc}, step,
                   {Red, Green, Blue, Orange, White, Yellow});
      }
    }
  }
  //  for (const auto &vert : builder.vertices) {
  //    std::cout << vert.position.x << '\t' << vert.position.y << '\t'
  //              << vert.position.z << std::endl;
  //  }

  return builder;
}

Color extractColor(const uint64_t &plane, int x, int y) {
  assert(x == 1 && y == 1 && "center values are const look ups");
}

Color RubiksCube::getColor(Face face, int x, int y, int z) {
  Face currentFace = Inside;
  if (x == 0 && face == Left) {
    currentFace = Left;
    if (y == 1 && z == 1) {
      return faceColors[currentFace - 1];
    }
    auto plane = data[currentFace - 1];
  }
}

} // namespace lve
