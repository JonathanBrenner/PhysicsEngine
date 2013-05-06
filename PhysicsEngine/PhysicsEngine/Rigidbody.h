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
    Rigidbody(float width, float height, float depth);
    
    void update();
    void calcCenterOfMass();
    
    GameObject* gameObject;
    
    // Solid Cuboid inertia tensor
    float width;
    float height;
    float depth;
    
    // Mass
    float mass;
    glm::vec3 centerOfMass;
    
    // Linear dynamics
    glm::vec3 momentum;
    glm::vec3 force;
    
    // Angular dynamics
    glm::mat3 inertiaTensor;
    glm::mat3 inverseInertiaTensor;
    glm::quat orientation;
    glm::vec3 displacement;
    glm::vec3 torque;
    glm::vec3 angularMomentum;
    glm::vec3 angularVelocity;
    
    bool enabled;

private:
    struct State
    {
        // Primary Quantities
        glm::vec3 position;
        glm::vec3 momentum;
        glm::quat orientation;
        glm::vec3 angularMomentum;
        
        // Secondary Quantities
        glm::vec3 velocity;
        glm::quat angularVelocity;
        glm::quat spin;
    };
    
    struct Derivative
    {
        glm::vec3 velocity;
        glm::quat spin;
    };

    void init();
    Derivative evaluate(State& state, float t, float dt, const Derivative& derivative);
    void integrate(State& state, float t, float dt);
    void recalculate(State& state);
};

#endif /* defined(__PhysicsEngine__Rigidbody__) */
