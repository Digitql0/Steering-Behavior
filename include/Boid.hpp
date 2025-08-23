#ifndef BOID_HPP
#define BOID_HPP

#include <raylib.h>
#include <raymath.h>
#include <string>
#include <unordered_map>
#include <vector>

struct Boid;
struct BoidGroup;
struct Behavior;
struct BoidGroupBehavior;

typedef struct Boid {
  Vector2 acceleration;
  Vector2 last_acceleration;
  Vector2 velocity;
  Vector2 position;

  float max_speed;
  float max_force;

  std::unordered_map<std::string, float> behavior_storage;

  std::unordered_map<std::string, Behavior> behaviors;
} Boid;

typedef struct BoidGroup {

  std::unordered_map<unsigned int, Boid> members;
  std::vector<unsigned int> free_ids;
  std::vector<unsigned int> used_ids;
  unsigned int next_id = 0;

  std::unordered_map<std::string, float> behavior_storage;

  std::unordered_map<std::string, Behavior> behaviors;
} BoidGroup;

typedef struct Behavior {
  Vector2 (*calculateForce)(void *, void*);
  void *target;
  float weight;
} Behavior;

typedef struct BoidGroupBehavior {
  Vector2 (*calculateForce)(BoidGroup *, void*);
  float weight;
} BoidGroupBehavior;

Boid make_boid(float max_speed, float max_force, Vector2 pos);
BoidGroup make_group(float seperation_radius);
unsigned int add_boid(BoidGroup* group, Boid boid);
void remove_boid(BoidGroup* group, unsigned int id);
void free_behavior_parameters(BoidGroup* group);
void apply_behaviors_boid(Boid *boid);
void apply_behaviors_group(BoidGroup* group);
Behavior *get_behavior_boid(std::string name, Boid* boid);
Behavior *get_behavior_group(std::string name, BoidGroup* group);
float* get_behavior_parameter_boid(std::string name, Boid* boid);
float* get_behavior_parameter_group(std::string name, BoidGroup* group);
std::vector<unsigned int> get_boid_ids(BoidGroup* group);
Boid* get_boid(BoidGroup* group, unsigned int id);
void get_boid_information(Boid* boid);
void apply_force(Vector2 force, float weight, Boid *boid);
void update(Boid *boid);

Vector2 Vector2Limit(Vector2 v, float limit);

Vector2 seek(void* boid, void* target);
Vector2 flee(void* boid, void* target);
Vector2 arrive(void* boid, void* target);
Vector2 pursue(void* boid, void* target);
Vector2 wander(void* boid, void* target);
Vector2 path_following(void* boid, void* target);
Vector2 obstacle_avoidance(void* boid, void* target);
Vector2 wall_following(void* boid, void* target);

Vector2 separation(void* group, void* target);
Vector2 cohesion(void* group, void* target);
Vector2 align(void* group, void* target);
Vector2 velocity_match(void* group, void* target);
Vector2 flocking(void* group, void* target);
Vector2 leader_following(void* group, void* target);
Vector2 queueing(void* group, void* target);

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
