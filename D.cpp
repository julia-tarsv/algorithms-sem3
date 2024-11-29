#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const int cKMax = 26;

class Convertor {
 private:
  static const char cKMinChar = 'a';

 public:
  static int CharToInt(char latter) {
    return static_cast<int>(latter - cKMinChar);
  }
};

int Log2(int num) {
  int degree = 0;
  int two_in_degree = 1;
  while (num > two_in_degree) {
    two_in_degree *= 2;
    degree += 1;
  }
  if (two_in_degree != num) {
    degree -= 1;
  }
  return degree;
}

void CountSort(const std::vector<int>& equal, std::vector<int>& permutation,
               int count) {
  std::vector<int> counts(equal.size());
  std::vector<int> new_permutation(equal.size());

  for (size_t i = 0; i < equal.size(); ++i) {
    size_t index = (i + (1 << (count - 1))) % equal.size();
    ++counts[equal[index]];
  }
  for (size_t i = 1; i < equal.size(); ++i) {
    counts[i] += counts[i - 1];
  }
  for (int i = (int)equal.size() - 1; i >= 0; --i) {
    size_t index = (i + (1 << (count - 1))) % equal.size();
    new_permutation[--counts[equal[index]]] = i;
  }

  counts.assign(equal.size(), 0);
  for (size_t i = 0; i < equal.size(); ++i) {
    ++counts[equal[i]];
  }
  for (size_t i = 1; i < equal.size(); ++i) {
    counts[i] += counts[i - 1];
  }
  for (int i = (int)equal.size() - 1; i >= 0; --i) {
    permutation[--counts[equal[new_permutation[i]]]] = new_permutation[i];
  }
}

void Calc(std::vector<int>& equal, const std::vector<int>& permutation,
          int count) {
  std::vector<int> new_equal(equal.size());
  for (size_t i = 1; i < equal.size(); ++i) {
    new_equal[permutation[i]] = new_equal[permutation[i - 1]];
    if (equal[permutation[i]] != equal[permutation[i - 1]] ||
        equal[(permutation[i] + (1 << (count - 1))) % equal.size()] !=
            equal[(permutation[i - 1] + (1 << (count - 1))) % equal.size()]) {
      ++new_equal[permutation[i]];
    }
  }
  equal = new_equal;
}

std::vector<int> Suff(std::string str) {
  int log2 = Log2(str.size());
  std::vector<int> counts(cKMax);

  for (size_t i = 0; i < str.size(); ++i) {
    ++counts[Convertor::CharToInt(str[i])];
  }
  for (size_t i = 1; i < cKMax; ++i) {
    counts[i] += counts[i - 1];
  }
  std::vector<int> permutation(str.size());
  for (int i = (int)str.size() - 1; i >= 0; --i) {
    permutation[--counts[Convertor::CharToInt(str[i])]] = i;
  }
  std::vector<int> equal(str.size());
  for (size_t i = 1; i < str.size(); ++i) {
    equal[permutation[i]] = equal[permutation[i - 1]];
    if (str[permutation[i]] != str[permutation[i - 1]]) {
      ++equal[permutation[i]];
    }
  }
  counts.assign(str.size(), 0);

  int count = 1;
  while (count <= log2 + 1) {
    CountSort(equal, permutation, count);
    Calc(equal, permutation, count);
    ++count;
  }
  return permutation;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  std::string str;
  std::cin >> str;
  std::vector<int> suff = Suff(str);
  std::string res;
  res.reserve(str.size());

  for (size_t i = 0; i < str.size(); ++i) {
    if (suff[i] == 0) {
      res += str[str.size() - 1];
    } else {
      res += str[suff[i] - 1];
    }
  }
  std::cout << res;
}
