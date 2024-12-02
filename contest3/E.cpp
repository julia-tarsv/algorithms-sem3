#include <iostream>

struct Poly {
  long long b;
  long long c;
  Poly(long long b = 0, long long c = 0) : b(b), c(c) {}
};

Poly Mult(const Poly& p1, const Poly& p2, long long mod, long long prime) {
  long long ac = (p1.b * p2.b) % prime;
  long long bd = (p1.c * p2.c) % prime;
  long long adbc = (p1.b * p2.c) % prime + (p1.c * p2.b) % prime;

  return Poly(adbc % prime, (bd + ac * mod) % prime);
}

Poly BinPow(Poly base, long long exp, long long mod, long long prime) {
  Poly res(0, 1);
  while (exp > 0) {
    if (exp % 2 == 1) {
      res = Mult(res, base, mod, prime);
    }
    base = Mult(base, base, mod, prime);
    exp /= 2;
  }
  return res;
}

long long ModInv(long long base, long long prime) {
  return BinPow(Poly(0, base), prime - 2, 0, prime).c;
}

long long ModExp(long long base, long long exp, long long prime) {
  long long res = 1;
  while (exp > 0) {
    if (exp % 2 == 1) {
      res = (res * base) % prime;
    }
    base = (base * base) % prime;
    exp /= 2;
  }
  return res;
}

int main() {
  size_t time;
  std::cin >> time;
  for (size_t i = 0; i < time; ++i) {
    long long mod;
    long long prime;
    std::cin >> mod >> prime;
    mod %= prime;

    if (prime == 2 || mod == 0) {
      std::cout << mod << '\n';
      continue;
    }

    if (ModExp(mod, (prime - 1) / 2, prime) != 1) {
      std::cout << "IMPOSSIBLE" << '\n';
      continue;
    }

    while (true) {
      long long rnd = rand() % (prime - 1) + 1;
      Poly pol = BinPow(Poly(1, rnd), (prime - 1) / 2, mod, prime);
      pol.c = (pol.c + prime - 1) % prime;

      if (pol.b != 0) {
        long long z = pol.c * ModInv(pol.b, prime) % prime;
        if (ModExp(z, 2, prime) == mod) {
          std::cout << z << '\n';
          break;
        }
      }
    }
  }
}