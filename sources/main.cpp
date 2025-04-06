#include <vector>

class Point {
public:
  double x, y;

  Point(double x = 0, double y = 0) : x(x), y(y) {}
};

class Triangle {
  public:
      Point a, b, c;
  
      Triangle(const Point& a, const Point& b, const Point& c) : a(a), b(b), c(c) {}
};

std::vector<Point> points;
std::vector<Triangle> triangles;

int WinMain(int argc, char** args) {
	return 0;
}