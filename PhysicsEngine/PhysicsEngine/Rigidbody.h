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
#include "glm.hpp"
#include "Transform.h"
#include "Time.h"

class GameObject;

class Rigidbody
{
public:
	Rigidbody();
    
    void update();
    
    GameObject* gameObject;

    glm::vec3 velocity;
    glm::vec3 forceApplied;
    float mass;             // kilograms
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

    glm::vec3 dPosition;
    glm::vec3 dVelocity;
};

#endif /* defined(__PhysicsEngine__Rigidbody__) */
