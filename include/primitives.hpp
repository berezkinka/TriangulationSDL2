#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <vector>
#include <cmath>

class Point {
  public:
    double x, y;
    Point(double x = 0, double y = 0);
    bool operator==(const Point& other) const;
};
    
class Edge {
  public:
    Point p1, p2;
    Edge(const Point& p1, const Point& p2);
    bool operator==(const Edge& other) const;
};
    
class Triangle {
  public:
    Point a, b, c;
    Edge ab, bc, ca;
    Triangle(const Point& a, const Point& b, const Point& c);
    bool operator==(const Triangle& other) const;
    std::vector<Edge> getEdges() const;
    bool hasVertex(const Point& p) const;
    bool hasEdge(const Edge& e) const;
};

#endif