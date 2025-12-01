#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>

using namespace std;

#define EPS 1e-9


struct Point {
  double x, y;
};

// DLL because any motion is to the immediate neighbour so we don't need O(1) access
struct Node {
  Point p;
  Node* prev;
  Node* next;
};

// Returns > 0 for counter-clockwise turn (p3 is to the left of p1-p2 vector)
// Returns < 0 for clockwise turn (p3 is to the right)
// Returns 0 for collinear points
double cross_product(Point a, Point b, Point c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

vector<Point> randomized_incremental_hull(vector<Point>& points) {
  if (points.size() <= 3) {
    if (points.size() == 3 && cross_product(points[0], points[1], points[2]) < 0)
      swap(points[1], points[2]);
    return points;
  }

  random_device rd;
  mt19937 g(rd());
  shuffle(points.begin(), points.end(), g);

  // Form triangle from first three points
  Node* n0 = new Node{points[0], nullptr, nullptr};
  Node* n1 = new Node{points[1], nullptr, nullptr};
  Node* n2 = new Node{points[2], nullptr, nullptr};

  // Anticlockwise
  if (cross_product(n0->p, n1->p, n2->p) < 0)
    swap(n1, n2);

  n0->next = n1; n1->prev = n0;
  n1->next = n2; n2->prev = n1;
  n2->next = n0; n0->prev = n2;

  Node* hull = n0;

  // Add the remaining points
  for (size_t i = 3; i < points.size(); ++i) {
    Point& pt = points[i];
        
    Node* visible_edge_start = nullptr;
    bool is_inside = true;
        
    // Find if the point is inside the current hull. If not, find a visible edge.
    Node* current_node = hull;
    do {
      if (cross_product(current_node->p, current_node->next->p, pt) < -EPS) {
        is_inside = false;
        visible_edge_start = current_node;
        break;
      }
      current_node = current_node->next;
    } while (current_node != hull);

    if (is_inside)
      continue;

    // Bring Me The Horizon (Tangent Points)
    Node* upper_tangent = visible_edge_start;
    while (cross_product(upper_tangent->prev->p, upper_tangent->p, pt) < -EPS)
      upper_tangent = upper_tangent->prev;

    Node* lower_tangent = visible_edge_start;
    while (cross_product(lower_tangent->p, lower_tangent->next->p, pt) < -EPS)
      lower_tangent = lower_tangent->next;

    // Connect the new node to the upper and lower tangents
    Node* new_node = new Node{pt, upper_tangent, lower_tangent};
    
    Node* start_delete = upper_tangent->next;
    
    upper_tangent->next = new_node;
    lower_tangent->prev = new_node;
        
    // Delete the nodes strictly between upper_tangent and lower_tangent
    Node* current_to_delete = start_delete;
    while (current_to_delete != lower_tangent) {
      Node* next_to_delete = current_to_delete->next;
      delete current_to_delete;
      current_to_delete = next_to_delete;
    }
        
    hull = new_node;
  }

  // Convert the linked list to a vector and clean up memory
  vector<Point> result;
  if (hull) {
    Node* start_node = hull;
    Node* current = hull;
    do {
      result.push_back(current->p);
      current = current->next;
    } while (current != start_node);

    current = start_node;
    while (true) {
      Node* next_node = current->next;
      delete current;
      if (next_node == start_node)
        break;
      current = next_node;
    }
  }
    
  return result;
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
  vector<Point> hull_points = randomized_incremental_hull(points);
  auto end = chrono::high_resolution_clock::now();
  chrono::duration<double> diff = end - start;
  cerr << "Time: " << diff.count() << " s" << endl;

  // Sort hull points for canonical output (starting from the lowest-then-leftmost point)
  if (!hull_points.empty()) {
    auto min_it = min_element(hull_points.begin(), hull_points.end(), [](const Point& a, const Point& b) {
      if (fabs(a.y - b.y) > EPS)
	return a.y < b.y;
      return a.x < b.x;
    });
    rotate(hull_points.begin(), min_it, hull_points.end());
  }

  cout << fixed << setprecision(2);
  for (const auto& p : hull_points)
    cout << p.x << " " << p.y << "\n";

  return 0;
}
