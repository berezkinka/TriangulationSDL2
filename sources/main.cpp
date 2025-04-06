#include <vector>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

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

SDL_Window *window;
SDL_Renderer *renderer;

bool init();
void kill();
bool loop();

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

bool loop()
{
  SDL_Event e;

  SDL_SetRenderDrawColor(renderer, 220, 220, 220, 220);
  SDL_RenderClear(renderer);

  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_QUIT:
      return false;
    case SDL_MOUSEBUTTONDOWN:
      double x = (double)e.button.x;
      double y = (double)e.button.y;
      points.emplace_back(x, y);
      break;
    }
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

int WinMain(int argc, char** args) {
	if (!init())
    return 1;

  while (loop()) {
    SDL_Delay(10);
  }

  kill();
  return 0;
}