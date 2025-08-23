#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <cstdlib>
#include <iostream>
#include <memory>
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

void render(Boid* boid, BoidGroup* group) {
  Vector2 Velocity = boid->velocity;
  Vector2 Position = boid->position;
  Vector2 LastAcceleration = boid->last_acceleration;

  if (Velocity.x != 0 && Velocity.y != 0)
    drawTriangle(Position, Vector2Normalize(Velocity));
  else
    DrawCircle(Position.x, Position.y, 10, RED);
  if (group != nullptr)
    DrawCircleLines(Position.x, Position.y,
                    *get_behavior_parameter_group("seperation_radius", group),
                    WHITE);

  Vector2 scaledVelocity = Vector2Scale(Velocity, 10);
  Vector2 shownVelocity = Vector2Add(Position, scaledVelocity);

  Vector2 scaledAcceleration = Vector2Scale(LastAcceleration, 10);
  Vector2 shownAcceleration = Vector2Add(Position, scaledAcceleration);

  DrawLine(Position.x, Position.y, shownVelocity.x, shownVelocity.y, BLUE);
  DrawLine(Position.x, Position.y, shownAcceleration.x, shownAcceleration.y,
           GREEN);
}

int main() {
  auto group = make_group(30);
  get_behavior_group("separation", &group)->weight = 1;
  InitWindow(800, 800, "lol");
  SetTargetFPS(60);

  Vector2 mousePos = {(float)GetMouseX(), (float)GetMouseY()};
  Boid spectator = make_boid(5, 0.5, {400, 400});
  get_behavior_boid("seek", &spectator)->target = &mousePos;
  get_behavior_boid("seek", &spectator)->weight = 1.0f;

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    mousePos = {(float)GetMouseX(), (float)GetMouseY()};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Boid boid = make_boid(5, 0.5, mousePos);
      get_behavior_boid("pursue", &boid)->target = &spectator;
      get_behavior_boid("pursue", &boid)->weight = 1.0f;
      auto id = add_boid(&group, boid);
      get_boid_information(get_boid(&group, id));
    }

    if (IsKeyPressed(KEY_UP)) {
      // get_behavior_group("separation", &group)->weight =
      //     get_behavior_group("separation", &group)->weight + 0.01f;
      float* v = get_behavior_parameter_group("seperation_radius", &group);
      *v = *v + 10;
    }

    if (IsKeyPressed(KEY_DOWN)) {
      // get_behavior_group("separation", &group)->weight =
      //     get_behavior_group("separation", &group)->weight - 0.01f;
      float* v = get_behavior_parameter_group("seperation_radius", &group);
      *v = *v - 10;
    }

    printf("SEPERATION: %f\n",
           get_behavior_group("separation", &group)->weight);

    apply_behaviors_boid(&spectator);
    update(&spectator);
    render(&spectator, nullptr);

    apply_behaviors_group(&group);

    // for (auto& boid : *boid_vector) {
    //   apply_behaviors_boid(&boid);
    //   printf("Applied Behaviors\n");
    //   BoidBehavior* beh = get_behavior_boid("seek", &boid);
    //   printf("%d\n", (beh->target != NULL));
    //   printf("weight: %f", beh->weight);
    // }

    auto boid_vector = get_boid_ids(&group);
    for (auto& id : boid_vector) {
      Boid* boid = get_boid(&group, id);
      // get_boid_information(boid);
      update(boid);
      render(boid, &group);
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
