#pragma once

#include <cstdint>
#include <vector>
#include <deque>

namespace seq {

inline std::vector<int64_t> bfs(int64_t start, std::vector<std::vector<int64_t>> const& edges) {
  std::vector<int64_t> dist(edges.size(), -1);
  dist[start] = 0;
  std::deque<int64_t> q;
  q.push_front(start);
  while (!q.empty()) {
    auto cur = q.front();
    q.pop_front();
    for (auto next: edges[cur]) {
      if (dist[next] == -1) {
        dist[next] = dist[cur] + 1;
        q.push_back(next);
      }
    }
  }
  return dist;
}

}
