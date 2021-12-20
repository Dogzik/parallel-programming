#include "src/cube_builder.h"

#include <cassert>

namespace {

void do_build_cube(std::vector<std::vector<int64_t>>& edges, const std::vector<int>& dimensions,
                   std::vector<int>& cur_coordinates) {
  if (cur_coordinates.size() == dimensions.size()) {
    auto u_node = get_node(cur_coordinates, dimensions);
    for (size_t i = 0; i < cur_coordinates.size(); ++i) {
      if (cur_coordinates[i] + 1 < dimensions[i]) {
        cur_coordinates[i] += 1;
        auto v_node = get_node(cur_coordinates, dimensions);
        edges[u_node].push_back(v_node);
        edges[v_node].push_back(u_node);
        cur_coordinates[i] -= 1;
      }
    }
  } else {
    size_t cur_dim = cur_coordinates.size();
    for (int i = 0; i < dimensions[cur_dim]; ++i) {
      cur_coordinates.push_back(i);
      do_build_cube(edges, dimensions, cur_coordinates);
      cur_coordinates.pop_back();
    }
  }
}

}

int64_t get_node(std::vector<int> const& coordinates, std::vector<int> const& dimensions) noexcept {
  assert(coordinates.size() == dimensions.size());
  int64_t elems_in_dimension = 1;
  int64_t res = 0;
  for (size_t i = 0; i < coordinates.size(); ++i) {
    auto real_idx = coordinates.size() - i - 1;
    res += elems_in_dimension * coordinates[real_idx];
    elems_in_dimension *= dimensions[real_idx];
  }
  return res;
}

int64_t nodes_cnt(std::vector<int> const& dimensions) noexcept {
  int64_t res = 1;
  for (auto dim: dimensions) {
    res *= dim;
  }
  return res;
}

std::vector<std::vector<int64_t>> build_cube(const std::vector<int>& dimensions) {
  auto total_nodes = nodes_cnt(dimensions);
  std::vector<std::vector<int64_t>> edges(total_nodes);
  std::vector<int> cur_coordinates;
  cur_coordinates.reserve(dimensions.size());
  do_build_cube(edges, dimensions, cur_coordinates);
  return edges;
}
