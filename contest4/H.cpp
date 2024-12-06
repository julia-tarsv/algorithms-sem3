#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct Point {
  long long x;
  long long y;
  Point() : x(0), y(0) {}
  Point(long long x, long long y) : x(x), y(y) {}
  Point operator+(const Point& other) const {
    return Point(x + other.x, y + other.y);
  }

  Point operator-(const Point& other) const {
    return Point(x - other.x, y - other.y);
  }
};

class Vector {
 public:
  Vector(const Point& end) : end_(end) {}
  Vector(const Vector& other) : end_(other.end_) {}
  Vector(const Point& start, const Point& end) : end_(end - start) {}

  Vector operator+(const Vector& other) const {
    return Vector(end_ + other.end_);
  }

  Vector operator-(const Vector& other) const {
    return Vector(end_ - other.end_);
  }

  long long AbsSqr() const { return end_.x * end_.x + end_.y * end_.y; }

  friend long long Vec(const Vector& l, const Vector& r);

  Point GetEnd() const { return end_; }

 private:
  Point end_;
};

class Polygon {
 public:
  Polygon(std::vector<Point>& vert) : vertices_(vert) {}
  long long DoubledArea();
  friend std::ostream& operator<<(std::ostream& os, const Polygon& vec);

 private:
  std::vector<Point> vertices_;
};

Point operator+(const Point& point, const Vector& other) {
  return point + other.GetEnd();
}

bool operator==(const Point& l, const Point& r) {
  return ((l.x == r.x) && (l.y == r.y));
}
bool operator!=(const Point& l, const Point& r) { return !(l == r); }

Point FindMinimal(const std::vector<Point>& points) {
  Point ans = points[0];
  for (size_t i = 1; i < points.size(); ++i) {
    if (points[i].x < ans.x) {
      ans = points[i];
    } else if (points[i].x == ans.x && points[i].y < ans.y) {
      ans = points[i];
    }
  }
  return ans;
}

class Cmp {
 public:
  Cmp(const Point& min) : minimal_(min) {}

  bool operator()(const Point& l, const Point& r) const {
    Vector vec1(minimal_, l);
    Vector vec2(minimal_, r);
    long long cross = Vec(vec1, vec2);
    if (cross == 0) {
      return vec1.AbsSqr() < vec2.AbsSqr();
    }
    return cross > 0;
  }

 private:
  Point minimal_;
};

void MinConvex(std::vector<Point>& convex, const std::vector<Point>& points) {
  size_t cur_index = 1;
  while (cur_index < points.size()) {
    if (points[cur_index] == *(convex.end() - 1)) {
      ++cur_index;
      continue;
    }
    if (convex.size() == 1) {
      convex.push_back(points[cur_index]);
      ++cur_index;
      continue;
    }
    Point last_point = *(convex.end() - 1);
    Point prev_last_point = *(convex.end() - 2);
    Vector vec1(prev_last_point, last_point);
    Vector vec2(last_point, points[cur_index]);
    if (Vec(vec1, vec2) <= 0) {
      convex.erase(convex.end() - 1);
    } else {
      convex.push_back(points[cur_index]);
      ++cur_index;
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
  os << point.x << " " << point.y;
  return os;
}

std::ostream& operator<<(std::ostream& os, const Polygon& vec) {
  os << vec.vertices_.size() << "\n";
  for (size_t i = 0; i < vec.vertices_.size(); ++i) {
    os << vec.vertices_[i] << "\n";
  }
  return os;
}

long long Vec(const Vector& l, const Vector& r) {
  return l.end_.x * r.end_.y - l.end_.y * r.end_.x;
}

long long Polygon::DoubledArea() {
  long long ans = 0;
  for (size_t i = 0; i < vertices_.size(); ++i) {
    ans += vertices_[i].x * vertices_[(i + 1) % vertices_.size()].y;
    ans -= vertices_[i].y * vertices_[(i + 1) % vertices_.size()].x;
  }
  return std::abs(ans);
}

int main() {
  size_t num;
  std::cin >> num;
  std::vector<Point> points(num);
  for (size_t i = 0; i < num; ++i) {
    int x;
    int y;
    std::cin >> x >> y;
    points[i] = Point(x, y);
  }
  Point min_point = FindMinimal(points);
  Cmp cmp(min_point);
  std::sort(points.begin(), points.end(), cmp);
  std::vector<Point> convex = {min_point};
  MinConvex(convex, points);
  Polygon pol(convex);
  std::cout << pol;
  long long area = pol.DoubledArea();
  if (area % 2 == 0) {
    std::cout << area / 2 << ".0";
  } else {
    std::cout << area / 2 << ".5";
  }
}
