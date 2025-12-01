#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

#define EPS 1e-9

struct Point {
  double x, y;
};

// Returns > 0 for counter-clockwise turn (p3 is to the left of p1-p2 vector)
// Returns < 0 for clockwise turn (p3 is to the right)
// Returns 0 for collinear points
double cross_product(Point a, Point b, Point c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

// Distance squared
double dist_sq(Point a, Point b) {
  return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

vector<Point> graham_scan(vector<Point> &points) {
  if (points.size() <= 2)
    return points;

  // 1. Find the bottom-most point (and left-most if ties)
  int min_idx = 0;
  for (size_t i = 1; i < points.size(); i++) {
    if (points[i].y < points[min_idx].y ||
        (fabs(points[i].y - points[min_idx].y) < EPS &&
         points[i].x < points[min_idx].x)) {
      min_idx = i;
    }
  }
  swap(points[0], points[min_idx]);
  Point p0 = points[0];

  // 2. Sort points by polar angle with respect to p0
  // If angles are same, sort by distance (closer first)
  sort(points.begin() + 1, points.end(), [&](const Point &a, const Point &b) {
    double cp = cross_product(p0, a, b);
    if (fabs(cp) > EPS)
      return cp > 0; // Counter-clockwise
    return dist_sq(p0, a) < dist_sq(p0, b);
  });

  // 3. Build the hull
  vector<Point> hull;
  hull.push_back(points[0]);
  hull.push_back(points[1]);

  for (size_t i = 2; i < points.size(); i++) {
    while (hull.size() >= 2) {
      Point p1 = hull.back();
      Point p2 = hull[hull.size() - 2];
      // If p2 -> p1 -> points[i] is NOT a Left turn (i.e. Right or Straight),
      // pop p1
      if (cross_product(p2, p1, points[i]) <= EPS)
        hull.pop_back();
      else
        break;
    }
    hull.push_back(points[i]);
  }

  return hull;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  vector<Point> points;
  double x, y;
  while (cin >> x >> y)
    points.push_back({x, y});

  if (points.empty())
    return 0;

  auto start = chrono::high_resolution_clock::now();
  vector<Point> hull_points = graham_scan(points);
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> diff = end - start;
  cerr << "Time: " << diff.count() << " s" << endl;

  cout << fixed << setprecision(2);
  for (const auto &p : hull_points)
    cout << p.x << " " << p.y << "\n";

  return 0;
}
