#pragma once

#include <atomic>
#include <functional>
#include <parray.hpp>
#include <datapar.hpp>

namespace par {

namespace detail {

static std::atomic_int64_t& to_atomic(int64_t& bare) noexcept {
  return reinterpret_cast<std::atomic_int64_t&>(bare);
}

template<typename C>
decltype(auto) back(C const& container) {
  return container[container.size() - 1];
}

}

inline pasl::pctl::parray<int64_t> bfs(int64_t start, std::vector<std::vector<int64_t>> const& edges) {
  pasl::pctl::parray<int64_t> dist(edges.size(), static_cast<int64_t>(-1));
  detail::to_atomic(dist[start]).store(0);
  pasl::pctl::parray<int64_t> cur_frontier{start};
  while (cur_frontier.size() != 0) {
    pasl::pctl::parray<uint64_t> frontier_nodes_sizes(
        cur_frontier.size(),
        [&edges, &cur_frontier](int64_t idx) {
          return edges[cur_frontier[idx]].size();
        }
    );
    auto pref_sizes = pasl::pctl::scan(
        frontier_nodes_sizes.begin(), frontier_nodes_sizes.end(), uint64_t{0},
        std::plus<uint64_t>{}, pasl::pctl::scan_type::forward_exclusive_scan
    );
    auto new_frontier_size = detail::back(pref_sizes) + detail::back(frontier_nodes_sizes);
    pasl::pctl::parray<int64_t> new_frontier(new_frontier_size, static_cast<int64_t>(-1));

    pasl::pctl::range::parallel_for(
        0L, cur_frontier.size(),
        [&pref_sizes, new_frontier_size](long l, long r) {
          auto pref_r = (r < pref_sizes.size()) ? pref_sizes[r] : new_frontier_size;
          return pref_r - pref_sizes[l];
        },
        [&](long idx) {
          auto cur_node = cur_frontier[idx];
          auto start_idx = pref_sizes[idx];
          for (size_t i = 0; i < edges[cur_node].size(); ++i) {
            auto new_node = edges[cur_node][i];
            auto new_dist = detail::to_atomic(dist[cur_node]).load() + 1;
            int64_t expected = -1;
            bool success = detail::to_atomic(dist[new_node]).compare_exchange_strong(expected, new_dist);
            if (success) {
              new_frontier[start_idx + i] = new_node;
            }
          }
        }
    );

    cur_frontier = pasl::pctl::filter(
        new_frontier.begin(), new_frontier.end(),
        [](int64_t cur_node) {
          return cur_node != -1;
        }
    );
  }
  return dist;
}

}
