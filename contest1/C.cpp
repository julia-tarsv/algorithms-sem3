#include <array>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

const int cKNeighbours = 26;
const char cFirstLetter = 'a';

struct Node {
  std::array<size_t, cKNeighbours> to;
  bool term;
  std::vector<size_t> index_str;
};

class Bor {
 public:
  std::vector<Node> bor;
  void Add(std::string& str, size_t index);
  size_t Size() const { return bor.size(); };
};

void Bor::Add(std::string& str, size_t index) {
  size_t vertex = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    if (bor[vertex].to[str[i] - cFirstLetter] == 0) {
      bor[vertex].to[str[i] - cFirstLetter] = bor.size();
      bor.push_back(Node());
    }
    vertex = bor[vertex].to[str[i] - cFirstLetter];
  }
  bor[vertex].index_str.push_back(index);
  bor[vertex].term = true;
}

void CntStep(const Bor& bor, std::vector<std::vector<size_t>>& step) {
  for (size_t i = 0; i < cKNeighbours; ++i) {
    if (bor.bor[0].to[i] != 0) {
      step[0][i] = bor.bor[0].to[i];
    } else {
      step[0][i] = 0;
    }
  }
}

void CntCompressed(const Bor& bor, std::vector<size_t>& compressed,
                   std::vector<std::vector<size_t>>& step,
                   std::queue<size_t>& q, std::vector<size_t>& link) {
  compressed[0] = 0;
  while (!q.empty()) {
    size_t vertex = q.front();
    q.pop();
    for (size_t i = 0; i < cKNeighbours; ++i) {
      if (bor.bor[vertex].to[i] == 0) {
        continue;
      }
      size_t u = bor.bor[vertex].to[i];
      link[u] = (vertex == 0 ? 0 : step[link[vertex]][i]);

      if (bor.bor[link[u]].term) {
        compressed[u] = link[u];
      } else {
        compressed[u] = compressed[link[u]];
      }

      for (size_t j = 0; j < cKNeighbours; ++j) {
        if (bor.bor[u].to[j] != 0) {
          step[u][j] = bor.bor[u].to[j];
        } else {
          step[u][j] = step[link[u]][j];
        }
      }
      q.push(u);
    }
  }
}

void AhoKorasik(const Bor& bor, std::vector<size_t>& compressed,
                const std::string& str, std::vector<std::vector<size_t>>& res,
                std::vector<int>& size,
                std::vector<std::vector<size_t>>& step) {
  size_t vert0 = 0;
  for (size_t i = 0; i <= str.size(); ++i) {
    size_t vert = compressed[vert0];
    while (vert != 0) {
      for (size_t j = 0; j < bor.bor[vert].index_str.size(); ++j) {
        res[bor.bor[vert].index_str[j]].push_back(
            i - size[bor.bor[vert].index_str[j]] + 1);
      }
      vert = compressed[vert];
    }
    if (bor.bor[vert0].term) {
      for (size_t j = 0; j < bor.bor[vert0].index_str.size(); ++j) {
        res[bor.bor[vert0].index_str[j]].push_back(
            i - size[bor.bor[vert0].index_str[j]] + 1);
      }
    }
    if (i != str.size()) {
      vert0 = step[vert0][str[i] - cFirstLetter];
    }
  }
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
  Bor bor;
  bor.bor.push_back({{}, false, {}});
  for (size_t i = 0; i < num; ++i) {
    std::string cur;
    std::cin >> cur;
    bor.Add(cur, i);
    size.push_back(cur.size());
  }

  std::vector<size_t> link(bor.Size());
  std::vector<std::vector<size_t>> step(bor.Size(),
                                        std::vector<size_t>(cKNeighbours));
  CntStep(bor, step);

  std::queue<size_t> q;
  link[0] = 0;
  q.push(0);
  std::vector<size_t> compressed(bor.Size());
  CntCompressed(bor, compressed, step, q, link);

  std::vector<std::vector<size_t>> res(num);
  AhoKorasik(bor, compressed, str, res, size, step);

  for (size_t i = 0; i < res.size(); ++i) {
    std::cout << res[i].size() << ' ';
    for (size_t j = 0; j < res[i].size(); ++j) {
      std::cout << res[i][j] << ' ';
    }
    std::cout << "\n";
  }
}
