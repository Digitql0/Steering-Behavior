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
  Vector2 Velocity = a.getVelocity();
  Vector2 Position = a.getPosition();
  Vector2 LastAcceleration = a.getLastAcceleration();
  if (Velocity.x != 0 && Velocity.y != 0)
    drawTriangle(Position, Vector2Normalize(Velocity));
  else
    DrawCircle(Position.x, Position.y, 10, RED);

  Vector2 scaledVelocity = Vector2Scale(Velocity, 10);
  Vector2 shownVelocity = Vector2Add(Position, scaledVelocity);

  Vector2 scaledAcceleration = Vector2Scale(LastAcceleration, 10);
  Vector2 shownAcceleration = Vector2Add(Position, scaledAcceleration);

  DrawLine(Position.x, Position.y, shownVelocity.x, shownVelocity.y, BLUE);
  DrawLine(Position.x, Position.y, shownAcceleration.x, shownAcceleration.y,
           GREEN);

  std::cout << "Position: " << Position.x << ", " << Position.y << std::endl;
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
      // if (Position.x > 800 || Position.x < 0 || Position.y > 800 ||
      //     Position.y < 0)
      //   Position = {400, 400};
      render(a);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
