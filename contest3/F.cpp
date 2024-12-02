#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

long long Rev(long long num, int size) {
  long long res = 0;
  for (int i = 0; i < size; ++i) {
    if ((num & (1LL << i)) != 0) {
      res |= 1LL << (size - 1 - i);
    }
  }
  return res;
}

class FFT {
 private:
  std::vector<std::complex<long double>> a_coeffs_;
  std::vector<std::complex<long double>> b_coeffs_;
  std::vector<std::complex<long double>> c_coeffs_;
  long long deg_;

  void Reverse(std::vector<std::complex<long double>>& array) const {
    int logn = std::log2(deg_);
    for (long long i = 0; i < deg_; ++i) {
      long long x = Rev(i, logn);
      if (i < x) {
        std::swap(array[i], array[x]);
      }
    }
  }

  void Prepare(const std::vector<long long>& a_coeffs,
               const std::vector<long long>& b_coeffs) {
    a_coeffs_.clear();
    b_coeffs_.clear();
    c_coeffs_.clear();
    for (long long i = 0; i < (long long)a_coeffs.size(); ++i) {
      a_coeffs_.push_back(a_coeffs[i]);
    }
    for (long long i = 0; i < (long long)b_coeffs.size(); ++i) {
      b_coeffs_.push_back(b_coeffs[i]);
    }
    std::reverse(a_coeffs_.begin(), a_coeffs_.end());
    std::reverse(b_coeffs_.begin(), b_coeffs_.end());
    deg_ = std::pow(
        2, (long long)std::log2(a_coeffs.size() + b_coeffs.size()) + 1);
    for (long long i = a_coeffs.size(); i < deg_; ++i) {
      a_coeffs_.push_back(0);
    }
    for (long long i = b_coeffs.size(); i < deg_; ++i) {
      b_coeffs_.push_back(0);
    }
    c_coeffs_.resize(deg_);
  }

  void Fft(std::vector<std::complex<long double>>& poly, bool is_reverse) {
    std::complex<long double> omega(
        std::polar((long double)1.0, (long double)2 * M_PI / poly.size()));
    if (is_reverse) {
      omega = std::complex<long double>(1.0) / omega;
    }
    Reverse(poly);
    for (long long len = 2; len <= deg_; len *= 2) {
      std::complex<long double> curr_point = omega;
      for (long long i = deg_; i > len; i /= 2) {
        curr_point *= curr_point;
      }

      for (long long start = 0; start < deg_; start += len) {
        long long right = start + len / 2;
        long long left = start;
        std::complex<long double> degree = 1;
        while (right != start + len) {
          std::complex<long double> temp1 = poly[left] + degree * poly[right];
          std::complex<long double> temp2 = poly[left] - degree * poly[right];
          poly[left] = temp1;
          poly[right] = temp2;
          degree *= curr_point;
          if (is_reverse) {
            poly[left] /= 2;
            poly[right] /= 2;
          }
          ++left;
          ++right;
        }
      }
    }
  }

 public:
  FFT() {}
  std::vector<long long> Multiply(const std::vector<long long>& a_coeffs,
                                  const std::vector<long long>& b_coeffs) {
    Prepare(a_coeffs, b_coeffs);
    Fft(a_coeffs_, false);
    Fft(b_coeffs_, false);
    for (long long i = 0; i < deg_; ++i) {
      c_coeffs_[i] = a_coeffs_[i] * b_coeffs_[i];
    }
    Fft(c_coeffs_, true);
    long long ind = deg_ - 1;
    const long double cKEps = 1e-6;
    const long double cKBorder = 0.5;
    while (std::fabs(std::real(c_coeffs_[ind])) < cKEps && ind >= 0) {
      --ind;
    }
    std::vector<long long> ans;
    for (long long i = ind; i >= 0; --i) {
      if (std::real(c_coeffs_[i]) <= -cKEps) {
        ans.push_back(std::real(c_coeffs_[i]) - cKBorder);
      } else {
        ans.push_back(std::real(c_coeffs_[i]) + cKBorder);
      }
    }
    if (ans.empty()) {
      ans.push_back(0);
    }
    return ans;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  long long deg_a;
  long long deg_b;
  std::cin >> deg_a;
  std::vector<long long> a_coeffs(deg_a + 1);
  for (size_t i = 0; i <= (size_t)deg_a; ++i) {
    std::cin >> a_coeffs[i];
  }
  std::cin >> deg_b;
  std::vector<long long> b_coeffs(deg_b + 1);
  for (size_t i = 0; i <= (size_t)deg_b; ++i) {
    std::cin >> b_coeffs[i];
  }

  FFT fft;
  std::vector<long long> c_coeffs = fft.Multiply(a_coeffs, b_coeffs);
  std::cout << c_coeffs.size() - 1 << " ";
  for (size_t i = 0; i < c_coeffs.size(); ++i) {
    std::cout << c_coeffs[i] << " ";
  }
}