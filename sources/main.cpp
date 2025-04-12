#include <vector>
#include <cmath>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

class Point {
public:
  double x, y;

  Point(double x = 0, double y = 0) : x(x), y(y) {}
  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

class Edge {
public:
  Point p1, p2;
  
  Edge(const Point& p1, const Point& p2) : p1(p1), p2(p2) {}
  bool operator==(const Edge& other) const {
    return (p1 == other.p1 && p2 == other.p2) || (p1 == other.p2 && p2 == other.p1);
  }
};

class Triangle {
public:
  Point a, b, c;
  Edge ab, bc, ca;
  
  Triangle(const Point& a, const Point& b, const Point& c) : a(a), b(b), c(c), ab(a, b), bc(b, c), ca(c, a) {}
  bool operator==(const Triangle& other) const {
    return (a == other.a && b == other.b && c == other.c) ||
           (a == other.a && b == other.c && c == other.b) ||
           (a == other.b && b == other.a && c == other.c) ||
           (a == other.b && b == other.c && c == other.a) ||
           (a == other.c && b == other.a && c == other.b) ||
           (a == other.c && b == other.b && c == other.a);
  }
  std::vector<Edge> getEdges() const {
    return {ab, bc, ca};
  }
  bool hasVertex(const Point& p) const {
    return a == p || b == p || c == p;
  }
  bool hasEdge(const Edge& e) const {
    return (ab == e) || (bc == e) || (ca == e);
  }
};

std::vector<Point> points;
std::vector<Triangle> triangles;

SDL_Window *window;
SDL_Renderer *renderer;

bool init();
void kill();
bool loop();
bool isPointInCircumcircle(const Point& p, const Triangle& tri);
void triangulation();

bool init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }

  window = SDL_CreateWindow("Triangulation SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
  if (!window) {
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    return false;
  }

  return true;
}

bool loop() {
  SDL_Event e;

  SDL_SetRenderDrawColor(renderer, 220, 220, 220, 220);
  SDL_RenderClear(renderer);

  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_QUIT:
      return false;
    case SDL_MOUSEBUTTONDOWN: {
      double x = (double)e.button.x;
      double y = (double)e.button.y;
      points.emplace_back(x, y);
      break;
    }
    case SDL_KEYDOWN: {
			switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_T: {
					triangles.clear();
					triangulation();
					break;
        }
        case SDL_SCANCODE_C: {
					points.clear();
					triangles.clear();
					break;
        }
      }
      break;
    }
    }
  }

  for (const auto& triangle : triangles) {
		aalineColor(renderer, (int)triangle.a.x, (int)triangle.a.y, (int)triangle.b.x, (int)triangle.b.y, 0xFFFF0000);
		aalineColor(renderer, (int)triangle.b.x, (int)triangle.b.y, (int)triangle.c.x, (int)triangle.c.y, 0xFFFF0000);
		aalineColor(renderer, (int)triangle.c.x, (int)triangle.c.y, (int)triangle.a.x, (int)triangle.a.y, 0xFFFF0000);
	}

  for (const auto &point : points) {
    aacircleColor(renderer, (int)point.x, (int)point.y, 3, 0xFF0000FF);
    filledCircleColor(renderer, (int)point.x, (int)point.y, 3, 0xFF0000FF);
  }

  SDL_RenderPresent(renderer);

  return true;
}

void kill() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

// Функция проверяет, что точка попадает в описанную окружность треугольника.
bool isPointInCircumcircle(const Point& p, const Triangle& tri) {
  double adx = tri.a.x - p.x, ady = tri.a.y - p.y;
  double bdx = tri.b.x - p.x, bdy = tri.b.y - p.y;
  double cdx = tri.c.x - p.x, cdy = tri.c.y - p.y;

  double det = (
    (adx * adx + ady * ady) * (bdx * cdy - bdy * cdx) -
    (bdx * bdx + bdy * bdy) * (adx * cdy - ady * cdx) +
    (cdx * cdx + cdy * cdy) * (adx * bdy - ady * bdx)
  );

  return det > 1e-10; // Порог для учёта ошибок округления.
}

void triangulation() {
  // Создаем супер-треугольник с запасом (он должен охватывать все точки).
  double minX = points[0].x, maxX = points[0].x;
  double minY = points[0].y, maxY = points[0].y;
  
  for (const auto& p : points) {
      minX = std::min(minX, p.x);
      maxX = std::max(maxX, p.x);
      minY = std::min(minY, p.y);
      maxY = std::max(maxY, p.y);
  }

  double dtx = maxX - minX, dty = maxY - minY;
  double margin = std::max(dtx, dty) * 10;

  Point a(minX - margin, minY - margin);
  Point b(maxX + margin, minY - margin);
  Point c(minX + dtx / 2, maxY + margin);

  Triangle superTriangle = Triangle(a, b, c);
  triangles.push_back(superTriangle);

  // Добавляем точки.
  for (const auto& point : points) {
    
    std::vector<Triangle> badTriangles; // Треугольники, в описанную окружность которых попадает точка.
    
    for (const auto& triangle : triangles) {
      if (isPointInCircumcircle(point, triangle)) {
          badTriangles.push_back(triangle);
      }
    }

    // Найдём ребра, которые принадлежат только одному такому треугольнику,
    // они образуют границу многоугольника.
    std::vector<Edge> polygonEdges;
    for (const auto& triangle : badTriangles) {
      std::vector<Edge> edges = triangle.getEdges();
      for (const auto& edge : edges) { 
        bool isShared = false;     
        for (const auto& otherTriangle : badTriangles) {
          if (triangle == otherTriangle) continue;
          if (otherTriangle.hasEdge(edge)) {
            isShared = true;
            break;
          }
        }
        if (!isShared) {
          polygonEdges.push_back(edge);
        }
      }
    }

    // Удаляем плохие треугольники.
    for (const auto& triangle : badTriangles) {
      std::erase(triangles, triangle);
    }

    // Добавляем новые треугольники из рёбер и точки point.
    for (const auto& edge : polygonEdges) {
      triangles.push_back(Triangle(edge.p1, edge.p2, point));
    }
  }
  // Удаляем треугольники, связанные с вершинами супер-треугольника.
  std::vector<Triangle> trianglesToRemove;
  for (const auto& triangle : triangles) {
    if (triangle.hasVertex(superTriangle.a) || triangle.hasVertex(superTriangle.b) || triangle.hasVertex(superTriangle.c)) {
      trianglesToRemove.push_back(triangle); 
    } 
  }
  for (const auto& triangle : trianglesToRemove) {
    std::erase(triangles, triangle);
  }
}

int WinMain(int argc, char** args) {
	if (!init())
    return 1;

  while (loop()) {
    SDL_Delay(10);
  }

  kill();
  return 0;
}