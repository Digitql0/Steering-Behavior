#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <vector>

#include "Agent.hpp"

void drawTriangle(Vector2 position, Vector2 direction) {
  direction = Vector2Normalize(direction);
  direction = Vector2Scale(direction, 20);
  Vector2 v1 = {-direction.y / 2, direction.x / 2};
  Vector2 v2 = {direction.x, direction.y};
  Vector2 v3 = {direction.y / 2, -direction.x / 2};
  v1 = Vector2Add(v1, position);
  v2 = Vector2Add(v2, position);
  v3 = Vector2Add(v3, position);
  DrawTriangle(v1, v2, v3, RED);
}

void render(const Agent& a) {
  if (a.Velocity.x != 0 && a.Velocity.y != 0)
    drawTriangle(a.Position, Vector2Normalize(a.Velocity));
  else
    DrawCircle(a.Position.x, a.Position.y, 10, RED);

  Vector2 scaledVelocity = Vector2Scale(a.Velocity, 10);
  Vector2 shownVelocity = Vector2Add(a.Position, scaledVelocity);

  Vector2 scaledAcceleration = Vector2Scale(a.LastAcceleration, 10);
  Vector2 shownAcceleration = Vector2Add(a.Position, scaledAcceleration);

  DrawLine(a.Position.x, a.Position.y, shownVelocity.x, shownVelocity.y, BLUE);
  DrawLine(a.Position.x, a.Position.y, shownAcceleration.x, shownAcceleration.y,
           GREEN);

  std::cout << "Position: " << a.Position.x << ", " << a.Position.y
            << std::endl;
}

int main() {
  std::vector<Agent> Bureau;
  Vector2 pos = {400, 400};
  Agent a(5, 0.5, 5, pos);
  Bureau.push_back(a);
  InitWindow(800, 800, "lol");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawLine(200, 200, 200, 400, GREEN);
    DrawLine(200, 200, 400, 200, BLUE);
    Vector2 mousePos = {(float)GetMouseX(), (float)GetMouseY()};
    for (auto& a : Bureau) {
      a.flee(mousePos, 1);
    }

    for (auto& a : Bureau) {
      a.update();
      if (a.Position.x > 800 || a.Position.x < 0 || a.Position.y > 800 ||
          a.Position.y < 0)
        a.Position = {400, 400};
      render(a);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
