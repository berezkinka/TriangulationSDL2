#include "primitives.hpp"

Point::Point(double x, double y) : x(x), y(y) {}

bool Point::operator==(const Point& other) const {
  return x == other.x && y == other.y;
}

Edge::Edge(const Point& p1, const Point& p2) : p1(p1), p2(p2) {}
  
bool Edge::operator==(const Edge& other) const {
  return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
}

Triangle::Triangle(const Point& a, const Point& b, const Point& c) : a(a), b(b), c(c), ab(a, b), bc(b, c), ca(c, a) {}
  
bool Triangle::operator==(const Triangle& other) const {
  return (a == other.a && b == other.b && c == other.c) ||
         (a == other.a && b == other.c && c == other.b) ||
         (a == other.b && b == other.a && c == other.c) ||
         (a == other.b && b == other.c && c == other.a) ||
         (a == other.c && b == other.a && c == other.b) ||
         (a == other.c && b == other.b && c == other.a);
}
  
std::vector<Edge> Triangle::getEdges() const {
  return {ab, bc, ca};
}

bool Triangle::hasVertex(const Point& p) const {
  return a == p || b == p || c == p;
}

bool Triangle::hasEdge(const Edge& e) const {
  return (ab == e) || (bc == e) || (ca == e);
}