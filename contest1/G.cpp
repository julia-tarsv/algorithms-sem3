#include <algorithm>
#include <iostream>
#include <map>
#include <set>

bool CheckPrefix(const std::string& str, size_t index) {
  for (size_t i = 0; i < index; ++i) {
    if (str[i] != str[index - 1 - i]) {
      return false;
    }
  }
  return true;
}

bool CheckSuffix(const std::string& str, size_t index) {
  for (size_t i = 0; i < index; ++i) {
    if (str[str.size() - 1 - i] != str[str.size() - index + i]) {
      return false;
    }
  }
  return true;
}

void Solve(std::map<std::string, int>& info) {
  std::set<std::pair<int, int>> answers;
  for (const auto& pair : info) {
    std::string curr_str = pair.first;
    for (size_t i = 0; i < curr_str.size(); ++i) {
      if (CheckPrefix(curr_str, i)) {
        std::string rev_suff = curr_str.substr(i);
        reverse(rev_suff.begin(), rev_suff.end());
        if (info.find(rev_suff) != info.end() &&
            info[rev_suff] != pair.second) {
          answers.insert({info[rev_suff], pair.second});
        }
      }
      if (CheckSuffix(curr_str, i)) {
        std::string rev_pref = curr_str.substr(0, curr_str.size() - i);
        reverse(rev_pref.begin(), rev_pref.end());
        if (info.find(rev_pref) != info.end() &&
            info[rev_pref] != pair.second) {
          answers.insert({pair.second, info[rev_pref]});
        }
      }
    }
  }

  std::cout << answers.size() << "\n";
  for (const auto& pair : answers) {
    std::cout << pair.first << " " << pair.second << "\n";
  }
}

int main() {
  int n;
  std::cin >> n;
  std::map<std::string, int> map;
  for (int i = 0; i < n; ++i) {
    std::string s;
    std::cin >> s;
    map[s] = i + 1;
  }
  Solve(map);
}