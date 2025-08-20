#include "Agent.hpp"

#include <raymath.h>

#include <iostream>

Agent::Agent(float maxspeed, float maxforce, float maxvelocity,
             Vector2 position)
    : Position(position),
      maxSpeed(maxspeed),
      maxForce(maxforce),
      maxVelocity(maxvelocity),
      LastAcceleration(Vector2Zero()),
      Velocity(Vector2Zero()),
      Acceleration(Vector2Zero()) {}

void Agent::applyForce(Vector2 force, float weight) {
  force = Vector2Scale(force, weight);
  Acceleration = Vector2Add(Acceleration, force);
}

void Agent::update() {
  Velocity = Vector2Add(Velocity, Acceleration);
  Velocity = Vector2Limit(Velocity, maxSpeed);
  Position = Vector2Add(Position, Velocity);
  LastAcceleration = Acceleration;
  Acceleration = Vector2Zero();
  std::cout << "VELOCITY: " << Velocity.x << ", " << Velocity.y
            << ", \nPOSITION: " << Position.x << ", " << Position.y
            << ", \nACCELERATION: " << LastAcceleration.x << ", "
            << LastAcceleration.y << std::endl;
}

void Agent::seek(Vector2 target, float weight) {
  Vector2 desired = Vector2Subtract(target, Position);
  desired = Vector2Limit(desired, maxVelocity);
  Vector2 steering = Vector2Subtract(desired, Velocity);
  steering = Vector2Limit(steering, maxForce);
  applyForce(steering, weight);
}

void Agent::flee(Vector2 target, float weight) {
  Vector2 desired = Vector2Subtract(target, Position);
  desired = Vector2Limit(desired, maxVelocity);
  Vector2 steering = Vector2Subtract(desired, Velocity);
  steering = Vector2Limit(steering, maxForce);
  steering = Vector2Scale(steering, -1);
  applyForce(steering, weight);
}

Vector2 Agent::Vector2Limit(Vector2 v, float limit) {
  float currLength = Vector2Length(v);
  if (currLength > limit) return Vector2Scale(Vector2Normalize(v), limit);
  return v;
}

Vector2 Agent::getAcceleration() const { return Acceleration; }
Vector2 Agent::getLastAcceleration() const { return LastAcceleration; }
Vector2 Agent::getPosition() const { return Position; }
Vector2 Agent::getVelocity() const { return Velocity; }
