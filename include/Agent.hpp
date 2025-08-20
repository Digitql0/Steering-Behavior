#ifndef AGENT_HPP
#define AGENT_HPP

#include <raylib.h>
#include <raymath.h>

class Agent {
public:
    float maxSpeed;
    float maxForce;
    float maxVelocity;

    Agent(float maxspeed, float maxforce, float maxvelocity, Vector2 position);
    void seek(Vector2 target, float weight);
    void flee(Vector2 target, float weight);
    void applyForce(Vector2 force, float weight);
    void update();

    //Utility Function
    Vector2 getAcceleration() const;
    Vector2 getLastAcceleration() const;
    Vector2 getPosition() const;
    Vector2 getVelocity() const;
    Vector2 Vector2Limit(Vector2 v, float limit);
private:
    Vector2 Acceleration;
    Vector2 LastAcceleration;
    Vector2 Velocity;
    Vector2 Position;
};

#endif
