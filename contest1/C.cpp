#include <array>
#include <iostream>
#include <map>
#include <queue>

const int cKMax = 26;

struct Node {
  std::array<size_t, cKMax> to;
  bool term;
  std::vector<size_t> index_str;
};

std::vector<Node> bor;
void Add(std::string& str, size_t index) {
  size_t vertex = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    if (bor[vertex].to[str[i] - 'a'] == 0) {
      bor[vertex].to[str[i] - 'a'] = bor.size();
      bor.push_back(Node());
    }
    vertex = bor[vertex].to[str[i] - 'a'];
  }
  bor[vertex].index_str.push_back(index);
  bor[vertex].term = true;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  std::string str;
  std::cin >> str;
  size_t num;
  std::cin >> num;

  std::vector<int> size;
  bor.push_back({{}, false, {}});
  for (size_t i = 0; i < num; ++i) {
    std::string cur;
    std::cin >> cur;
    Add(cur, i);
    size.push_back(cur.size());
  }

  std::vector<size_t> link(bor.size());
  std::vector<std::vector<size_t>> step(bor.size(), std::vector<size_t>(cKMax));
  std::queue<size_t> q;
  link[0] = 0;
  for (size_t i = 0; i < cKMax; ++i) {
    if (bor[0].to[i] != 0) {
      step[0][i] = bor[0].to[i];
    } else {
      step[0][i] = 0;
    }
  }

  q.push(0);
  std::vector<size_t> compressed(bor.size());
  compressed[0] = 0;
  while (!q.empty()) {
    size_t vertex = q.front();
    q.pop();
    for (size_t i = 0; i < cKMax; ++i) {
      if (bor[vertex].to[i] == 0) {
        continue;
      }
      size_t u = bor[vertex].to[i];
      link[u] = (vertex == 0 ? 0 : step[link[vertex]][i]);

      if (bor[link[u]].term) {
        compressed[u] = link[u];
      } else {
        compressed[u] = compressed[link[u]];
      }

      for (size_t j = 0; j < cKMax; ++j) {
        if (bor[u].to[j] != 0) {
          step[u][j] = bor[u].to[j];
        } else {
          step[u][j] = step[link[u]][j];
        }
      }
      q.push(u);
    }
  }

  size_t vert0 = 0;
  std::vector<std::vector<size_t>> res(num);
  for (size_t i = 0; i <= str.size(); ++i) {
    size_t vert = compressed[vert0];
    while (vert != 0) {
      for (size_t j = 0; j < bor[vert].index_str.size(); ++j) {
        res[bor[vert].index_str[j]].push_back(i - size[bor[vert].index_str[j]] +
                                              1);
      }
      vert = compressed[vert];
    }
    if (bor[vert0].term) {
      for (size_t j = 0; j < bor[vert0].index_str.size(); ++j) {
        res[bor[vert0].index_str[j]].push_back(
            i - size[bor[vert0].index_str[j]] + 1);
      }
    }
    if (i != str.size()) {
      vert0 = step[vert0][str[i] - 'a'];
    }
  }

  for (size_t i = 0; i < res.size(); ++i) {
    std::cout << res[i].size() << ' ';
    for (size_t j = 0; j < res[i].size(); ++j) {
      std::cout << res[i][j] << ' ';
    }
    std::cout << "\n";
  }
}