#include "Boid.hpp"

#include <raymath.h>

#include <algorithm>
#include <cmath>

Boid make_boid(float max_speed, float max_force, Vector2 pos) {
  Boid boid;
  boid.acceleration = Vector2Zero();
  boid.last_acceleration = Vector2Zero();
  boid.velocity = Vector2Zero();
  boid.position = pos;
  boid.max_speed = max_speed;
  boid.max_force = max_force;

  Behavior seek_behavior = {seek, nullptr, 0};
  Behavior flee_behavior = {flee, nullptr, 0};
  Behavior arrive_behavior = {arrive, nullptr, 0};
  Behavior pursue_behavior = {pursue, nullptr, 0};
  Behavior wander_behavior = {wander, nullptr, 0};
  Behavior path_following_behavior = {path_following, nullptr, 0};
  Behavior obstacle_avoidance_behavior = {obstacle_avoidance, nullptr, 0};
  Behavior wall_following_behavior = {wall_following, nullptr, 0};

  boid.behaviors["seek"] = seek_behavior;
  boid.behaviors["flee"] = flee_behavior;
  boid.behaviors["arrive"] = arrive_behavior;
  boid.behaviors["pursue"] = pursue_behavior;
  boid.behaviors["wander"] = wander_behavior;
  boid.behaviors["path_following"] = path_following_behavior;
  boid.behaviors["obstacle_avoidance"] = obstacle_avoidance_behavior;
  boid.behaviors["wall_following"] = wall_following_behavior;

  return boid;
}

BoidGroup make_group(float seperation_radius) {
  BoidGroup group;

  group.behavior_storage["seperation_radius"] = seperation_radius;

  Behavior separation_behavior = {separation, nullptr, 0};
  Behavior cohesion_behavior = {cohesion, nullptr, 0};
  Behavior align_behavior = {align, nullptr, 0};
  Behavior velocity_match_behavior = {velocity_match, nullptr, 0};
  Behavior flocking_behavior = {flocking, nullptr, 0};
  Behavior leader_following_behavior = {leader_following, nullptr, 0};
  Behavior queueing_behavior = {queueing, nullptr, 0};

  group.behaviors["separation"] = separation_behavior;
  group.behaviors["cohesion"] = cohesion_behavior;
  group.behaviors["align"] = align_behavior;
  group.behaviors["velocity_match"] = velocity_match_behavior;
  group.behaviors["flocking"] = flocking_behavior;
  group.behaviors["leader_following"] = leader_following_behavior;
  group.behaviors["queueing"] = queueing_behavior;

  return group;
};

unsigned int add_boid(BoidGroup* group, Boid boid) {
  int new_id;
  if (group->free_ids.empty()) {
    new_id = group->next_id++;
  } else {
    new_id = group->free_ids.back();
    group->free_ids.pop_back();
  }
  group->used_ids.push_back(new_id);

  group->members[new_id] = boid;

  return new_id;
}

void remove_boid(BoidGroup* group, unsigned int id) {
  group->members.erase(id);
  auto it = std::find(group->used_ids.begin(), group->used_ids.end(), id);
  if (it != group->used_ids.end()) {
    group->used_ids.erase(it);
  }
  group->free_ids.push_back(id);
}

void apply_behaviors_boid(Boid* boid) {
  for (auto& [str, behavior] : boid->behaviors) {
    if (behavior.calculateForce != nullptr && behavior.target != nullptr) {
      Vector2 desired = behavior.calculateForce(boid, behavior.target);
      desired = Vector2Limit(desired, boid->max_speed);
      Vector2 steering = Vector2Subtract(desired, boid->velocity);
      steering = Vector2Limit(steering, boid->max_force);
      apply_force(steering, behavior.weight, boid);
    }
  }
}

void apply_behaviors_group(BoidGroup* group) {
  for (auto& [id, boid] : group->members) {
    for (auto& [str, behavior] : group->behaviors) {
      if (behavior.calculateForce != nullptr) {
        std::pair<unsigned int, Boid*> target = {id, &boid};
        Vector2 desired = behavior.calculateForce(group, &target);
        desired = Vector2Limit(desired, boid.max_speed);
        Vector2 steering = Vector2Subtract(desired, boid.velocity);
        steering = Vector2Limit(steering, boid.max_force);
        if (std::isnan(steering.x) || std::isnan(steering.y)) {
          printf("%s: is nan\n", str.c_str());
          continue;
        }
        apply_force(steering, behavior.weight, &boid);
      }
    }
    for (auto& [str, behavior] : boid.behaviors) {
      if (behavior.calculateForce != nullptr && behavior.target != nullptr) {
        Vector2 desired = behavior.calculateForce(&boid, behavior.target);
        desired = Vector2Limit(desired, boid.max_speed);
        Vector2 steering = Vector2Subtract(desired, boid.velocity);
        steering = Vector2Limit(steering, boid.max_force);
        apply_force(steering, behavior.weight, &boid);
      }
    }
  }
}

Behavior* get_behavior_boid(std::string name, Boid* boid) {
  auto index = boid->behaviors.find(name);
  if (index != boid->behaviors.end()) return &index->second;
  return nullptr;
}

Behavior* get_behavior_group(std::string name, BoidGroup* group) {
  auto index = group->behaviors.find(name);
  if (index != group->behaviors.end()) return &index->second;
  return nullptr;
}

float* get_behavior_parameter_boid(std::string name, Boid* boid) {
  auto index = boid->behavior_storage.find(name);
  if (index != boid->behavior_storage.end()) return &index->second;
  return nullptr;
}

float* get_behavior_parameter_group(std::string name, BoidGroup* group) {
  auto index = group->behavior_storage.find(name);
  if (index != group->behavior_storage.end()) return &index->second;
  return nullptr;
}

Boid* get_boid(BoidGroup* group, unsigned int id) {
  auto it = group->members.find(id);
  if (it != group->members.end()) {
    return &it->second;
  }
  return nullptr;
}

void get_boid_information(Boid* boid) {
  printf(
      "NEW POSITION: %f, %f\nVELOCITY: %f, "
      "%f\nACCELERATION: %f, %f\n",
      boid->position.x, boid->position.y, boid->velocity.x, boid->velocity.y,
      boid->acceleration.x, boid->acceleration.y);
}

std::vector<unsigned int> get_boid_ids(BoidGroup* group) {
  return group->used_ids;
}

void apply_force(Vector2 force, float weight, Boid* boid) {
  boid->acceleration =
      Vector2Add(boid->acceleration, Vector2Scale(force, weight));
}

void update(Boid* boid) {
  boid->velocity = Vector2Add(boid->velocity, boid->acceleration);
  boid->position = Vector2Add(boid->position, boid->velocity);
  boid->last_acceleration = boid->acceleration;
  boid->acceleration = Vector2Zero();
}

Vector2 Vector2Limit(Vector2 v, float limit) {
  float length = Vector2Length(v);
  if (length > limit) v = Vector2Scale(Vector2Normalize(v), limit);
  return v;
}

Vector2 seek(void* boid, void* target) {
  Vector2* target_vector = (Vector2*)target;
  Boid* param_boid = (Boid*)boid;
  Vector2 desired = Vector2Subtract(*target_vector, param_boid->position);
  desired = Vector2Scale(Vector2Normalize(desired), param_boid->max_speed);
  return desired;
}

Vector2 flee(void* boid, void* target) {
  Vector2 desired = seek(boid, target);
  desired = Vector2Scale(desired, -1);
  return desired;
}

Vector2 arrive(void* boid, void* target) { return Vector2Zero(); }

Vector2 pursue(void* boid, void* target) {
  Boid* target_boid = (Boid*)target;
  Boid* param_boid = (Boid*)boid;
  Vector2 next_position =
      Vector2Add(target_boid->velocity, target_boid->position);
  Vector2 desired = Vector2Subtract(next_position, param_boid->position);
  return desired;
}

Vector2 wander(void* boid, void* target) { return Vector2Zero(); }

Vector2 path_following(void* boid, void* target) { return Vector2Zero(); }

Vector2 obstacle_avoidance(void* boid, void* target) { return Vector2Zero(); }

Vector2 wall_following(void* boid, void* target) { return Vector2Zero(); }

Vector2 separation(void* group, void* target) {
  BoidGroup* _group = (BoidGroup*)group;
  auto* _target = (std::pair<unsigned int, Boid*>*)target;
  Boid* self = _target->second;

  Vector2 center = Vector2Zero();
  int count = 0;

  for (auto id : get_boid_ids(_group)) {
    if (id == _target->first) continue;  // skip self
    Boid* boid = get_boid(_group, id);

    float seperation_radius =
        *get_behavior_parameter_group("seperation_radius", _group);

    if (Vector2Distance(self->position, boid->position) <= seperation_radius) {
      center = Vector2Add(center, boid->position);
      count++;
    }
  }

  if (count == 0) return self->velocity;  // no neighbors

  center = Vector2Scale(center, 1.0f / count);  // average position
  return flee(self, &center);                   // flee from center
}

Vector2 cohesion(void* group, void* target) { return Vector2Zero(); }

Vector2 align(void* group, void* target) { return Vector2Zero(); }

Vector2 velocity_match(void* group, void* target) { return Vector2Zero(); }

Vector2 flocking(void* group, void* target) { return Vector2Zero(); }

Vector2 leader_following(void* group, void* target) { return Vector2Zero(); }

Vector2 queueing(void* group, void* target) { return Vector2Zero(); }
