#include <gtest/gtest.h>

#include "src/sequential/bfs.h"
#include "src/parallel/bfs.h"
#include "src/cube_builder.h"

namespace {

int64_t get_dist(std::vector<int> const& a, std::vector<int> const& b) noexcept {
  assert(a.size() == b.size());
  int64_t res = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    res += std::abs(a[i] - b[i]);
  }
  return res;
}

void do_get_all_nodes(std::vector<std::vector<int>>& nodes, std::vector<int> const& dimensions,
                      std::vector<int>& cur_coordinates) {
  if (cur_coordinates.size() == dimensions.size()) {
    nodes.push_back(cur_coordinates);
  } else {
    size_t cur_dim = cur_coordinates.size();
    for (int i = 0; i < dimensions[cur_dim]; ++i) {
      cur_coordinates.push_back(i);
      do_get_all_nodes(nodes, dimensions, cur_coordinates);
      cur_coordinates.pop_back();
    }
  }
}

std::vector<std::vector<int>> get_all_nodes(std::vector<int> const& dimensions) {
  std::vector<std::vector<int>> nodes;
  nodes.reserve(nodes_cnt(dimensions));
  std::vector<int> cur_coordinates;
  cur_coordinates.reserve(dimensions.size());
  do_get_all_nodes(nodes, dimensions, cur_coordinates);
  return nodes;
}

template<typename BFSFun>
void test_bfs(std::vector<int> const& dimensions, BFSFun bfs) {
  auto edges = build_cube(dimensions);
  auto nodes = get_all_nodes(dimensions);
  for (auto const &start : nodes) {
    auto start_idx = get_node(start, dimensions);
    auto dist = bfs(start_idx, edges);
    for (auto const &finish : nodes) {
      auto finish_idx = get_node(finish, dimensions);
      auto real_dist = get_dist(start, finish);
      ASSERT_EQ(real_dist, dist[finish_idx]);
    }
  }
}

}

TEST(seq_bfs, test2D) {
  test_bfs({4, 7}, seq::bfs);
}

TEST(seq_bfs, test3D) {
  test_bfs({5, 10, 7}, seq::bfs);
}

TEST(par_bfs, test2D) {
  test_bfs({10, 30}, par::bfs);
}

TEST(par_bfs, test3D) {
  test_bfs({10, 20, 30}, par::bfs);
}
