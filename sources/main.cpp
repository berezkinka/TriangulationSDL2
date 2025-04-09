#include <vector>
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
  std::vector<Edge> getEdges() const {
    return {ab, bc, ca};
  }
  bool hasEdge(const Edge& other) {
    return (ab == other) || (bc == other) || (ca == other);
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
  double ax = tri.a.x - p.x, ay = tri.a.y - p.y;
  double bx = tri.b.x - p.x, by = tri.b.y - p.y;
  double cx = tri.c.x - p.x, cy = tri.c.y - p.y;

  double det = (
      (ax * ax + ay * ay) * (bx * cy - by * cx) -
      (bx * bx + by * by) * (ax * cy - ay * cx) +
      (cx * cx + cy * cy) * (ax * by - ay * bx)
  );

  return det > 0;
}

void triangulation() {
  // Создаем супер-треугольник (он должен охватывать все точки).
  double minX = points[0].x, maxX = points[0].x;
  double minY = points[0].y, maxY = points[0].y;
  
  for (const auto& p : points) {
      minX = std::min(minX, p.x);
      maxX = std::max(maxX, p.x);
      minY = std::min(minY, p.y);
      maxY = std::max(maxY, p.y);
  }

  double dtx = maxX - minX, dty = maxY - minY;
  Point a(minX - dtx, minY - dty);
  Point b(maxX + dtx, minY - dty);
  Point c(minX + dtx / 2, maxY + dty);

  triangles.push_back(Triangle(a, b, c));

  // Добавляем точки.
  for (const auto& point : points) {
    
    std::vector<Triangle> badTriangles; // Треугольники, в описанную окружность которых попадает точка.
    
    for (const auto& triangle : triangles) {
      if (isPointInCircumcircle(point, triangle)) {
          badTriangles.push_back(triangle);
      }
    }
    // Работа с рёбрами. 

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