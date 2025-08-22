#include "Boid.hpp"

#include <raymath.h>

Boid makeBoid(float max_speed, float max_force, Vector2 pos) {
  Boid boid;
  boid.acceleration = Vector2Zero();
  boid.lastAcceleration = Vector2Zero();
  boid.velocity = Vector2Zero();
  boid.position = pos;
  boid.maxSpeed = max_speed;
  boid.maxForce = max_force;

  Behavior seekBehavior = {seek, NULL, 0};
  boid.behaviors["seek"] = seekBehavior;
  return boid;
}
void applyBehaviors(Boid *boid) {
  for (auto &[str, behavior] : boid->behaviors) {
    printf("str: %s\n", str.c_str());
    if (behavior.calculateForce != NULL && behavior.target != NULL) {
      Vector2 desired = behavior.calculateForce(boid, behavior.target);
      desired = Vector2Limit(desired, boid->maxSpeed);
      Vector2 steering = Vector2Subtract(desired, boid->velocity);
      steering = Vector2Limit(steering, boid->maxForce);
      applyForce(steering, behavior.weight, boid);
      printf("Applied Force");
    }
  }
}
Behavior *getBehavior(std::string name, Boid *boid) {
  auto index = boid->behaviors.find(name);
  if (index != boid->behaviors.end()) return &index->second;
  return nullptr;
}
void applyForce(Vector2 force, float weight, Boid *boid) {
  boid->acceleration =
      Vector2Add(boid->acceleration, Vector2Scale(force, weight));
}
void update(Boid *boid) {
  boid->velocity = Vector2Add(boid->velocity, boid->acceleration);
  boid->position = Vector2Add(boid->position, boid->velocity);
  boid->lastAcceleration = boid->acceleration;
  boid->acceleration = Vector2Zero();
}

Vector2 Vector2Limit(Vector2 v, float limit) {
  float length = Vector2Length(v);
  if (length > limit) v = Vector2Scale(Vector2Normalize(v), limit);
  return v;
}

Vector2 seek(Boid *boid, void *target) {
  Vector2 *target_vector = (Vector2 *)target;
  Vector2 desired = Vector2Subtract(*target_vector, boid->position);
  return desired;
}
