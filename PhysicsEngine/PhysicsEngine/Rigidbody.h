//
//  Rigidbody.h
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#ifndef __PhysicsEngine__Rigidbody__
#define __PhysicsEngine__Rigidbody__

#include <iostream>
#include <vector>
#include "glm.hpp"
#include "quaternion.hpp"
#include "Transform.h"
#include "Time.h"
#include "CollisionPoint.h"

class GameObject;

class Rigidbody
{
public:
	Rigidbody();
    
    void update();
    void onCollision(std::vector<CollisionPoint> collisionPoints);
    
    GameObject* gameObject;
    
    // Linear dynamics
    glm::vec3 velocity;
    glm::vec3 forceApplied;
    float mass;
    
    // Angular dynamics
    glm::mat3 momentOfInertia;
    glm::quat orientation;
    glm::vec3 angularVelocity;
    glm::vec3 displacement;
    glm::vec3 torque;
    glm::vec3 angularMomentum;
    
    bool enabled;

private:
    struct State
    {
        glm::vec3 position;
        glm::vec3 velocity;
    };
    
    struct Derivative
    {
        glm::vec3 dPosition;
        glm::vec3 dVelocity;
    };

    Derivative evaluate(const State& initial, float t, float dt, const Derivative& d);
    void integrate(State& state, float t, float dt);
};

#endif /* defined(__PhysicsEngine__Rigidbody__) */
