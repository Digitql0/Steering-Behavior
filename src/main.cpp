#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "Boid.hpp"

int randomInt(int min, int max) { return min + rand() % ((max + 1) - min); }

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

void render(Boid* boid) {
  Vector2 Velocity = boid->velocity;
  Vector2 Position = boid->position;
  Vector2 LastAcceleration = boid->lastAcceleration;

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
  std::vector<Boid> boidGroup;
  InitWindow(800, 800, "lol");
  SetTargetFPS(60);
  Vector2 mousePos = {(float)GetMouseX(), (float)GetMouseY()};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    mousePos = {(float)GetMouseX(), (float)GetMouseY()};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Boid boid = makeBoid(5, 0.5, mousePos);
      printf("Made Boid\n");
      getBehavior("seek", &boid)->target = &mousePos;
      printf("Changed Target for Seek Behavior\n");
      getBehavior("seek", &boid)->weight = 1.0f;
      printf("Changed weight for seek BEhavior\n");
      boidGroup.push_back(boid);
      printf("added boid to boidGroup\n");
    }

    for (auto& boid : boidGroup) {
      applyBehaviors(&boid);
      printf("Applied Behaviors\n");
      Behavior* beh = getBehavior("seek", &boid);
      printf("%d\n", (beh->target != NULL));
      printf("weight: %f", beh->weight);
    }

    for (auto& boid : boidGroup) {
      update(&boid);
      render(&boid);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
