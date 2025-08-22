#ifndef BOID_HPP
#define BOID_HPP

#include <raylib.h>
#include <raymath.h>
#include <string>
#include <unordered_map>

struct Boid;
struct Behavior;

typedef struct Boid {
  Vector2 acceleration;
  Vector2 lastAcceleration;
  Vector2 velocity;
  Vector2 position;

  float maxSpeed;
  float maxForce;

  std::unordered_map<std::string, void *> behavior_storage;

  std::unordered_map<std::string, Behavior> behaviors;
} Boid;

typedef struct Behavior {
  Vector2 (*calculateForce)(Boid *, void*);
  void *target;
  float weight;
} Behavior;

Boid makeBoid(float max_speed, float max_force, Vector2 pos);
void applyBehaviors(Boid *boid);
Behavior *getBehavior(std::string name, Boid* boid);
void applyForce(Vector2 force, float weight, Boid *boid);
void update(Boid *boid);

Vector2 Vector2Limit(Vector2 v, float limit);

Vector2 seek(Boid* boid, void* target);

// seek
// flee
// arrive
// pursue
// wander
// pathfollowing
// void obstacleAvoidance();
// void wallFollowing();

// // Group Behavior
// void separation();
// void cohesion();
// void align(std::vector<Agent> agents, float weight);
// void velocityMatch(std::vector<Agent> agents, float weight);
// void flocking();
// void leaderFollowing();
// void Queueing();

// // Advanced Behaviors
// void interpose();
// void hide();
// void offsetPursuit();
// void followPath();
// void containment();
// void lookWhereYoureGoing();
// void face();
// void avoidCrowd();

#endif
