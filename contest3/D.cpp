#include <cmath>
#include <iostream>
#include <map>

int Pow(int a, int b, int m) {
  int res = 1;
  while (b > 0) {
    if ((b & 1) != 0) {
      res = (res * 1LL * a) % m;
      --b;
    } else {
      a = (a * 1LL * a) % m;
      b >>= 1;
    }
  }
  return res % m;
}

int Solve(int a, int b, int m) {
  int n = (int)sqrt(m) + 1;
  std::map<int, int> vals;
  for (int i = n; i >= 1; --i) {
    vals[Pow(a, i * n, m)] = i;
  }
  const int cKAns = 2e9;
  int ans = cKAns;
  for (size_t i = 0; i <= (size_t)n; ++i) {
    int cur = (Pow(a, i, m) * 1LL * b) % m;
    if (static_cast<int>(vals.contains(cur)) != 0) {
      int x = vals[cur] * n - i;
      if (x < m) {
        ans = std::min(x, ans);
      }
    }
  }
  if (ans == cKAns) {
    return -1;
  }
  return ans;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int p;
  int b;
  int n;

  while (std::cin >> p >> b >> n) {
    int ans = Solve(b, n, p);

    if (ans == -1) {
      std::cout << "no solution"
                << "\n";
    } else {
      std::cout << ans << "\n";
    }
  }
}