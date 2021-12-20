#pragma once

#include <vector>
#include <cstdint>

int64_t get_node(std::vector<int> const& coordinates, std::vector<int> const& dimensions) noexcept;
int64_t nodes_cnt(std::vector<int> const& dimensions) noexcept;
std::vector<std::vector<int64_t>> build_cube(std::vector<int> const& dimensions);
