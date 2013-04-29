//
//  Rigidbody.cpp
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "Rigidbody.h"
#include "GameObject.h"

Rigidbody::Rigidbody()
{
    enabled = false;
	mass = 1;
    velocity = glm::vec3(0, 0, 0);
    forceApplied = glm::vec3(0, 0, 0);
}

void Rigidbody::update()
{
    Rigidbody::State state;
    state.position = gameObject->transform.position;
    state.velocity = velocity;
    integrate(state, Time::time, Time::deltaTime);
    glm::vec3 translation = state.position - gameObject->transform.position;
    velocity = state.velocity;
    gameObject->transform.translate(translation);
    
    orientation = glm::quat_cast(gameObject->transform.modelMatrix);
    //std::cout << orientation.x << ", " << orientation.y << ", " << orientation.z << ", " << orientation.w << std::endl;
}

void Rigidbody::onCollision(std::vector<CollisionPoint> collisionPoints)
{
    
}

Rigidbody::Derivative Rigidbody::evaluate(const Rigidbody::State &initial, float t, float dt, const Rigidbody::Derivative &d)
{
    Rigidbody::State state;
    state.position = initial.position + d.dPosition * dt;
    state.velocity = initial.velocity + d.dVelocity * dt;
    
    Rigidbody::Derivative output;
    output.dPosition = state.velocity;
    output.dVelocity = forceApplied / mass;
    
    return output;
}

void Rigidbody::integrate(State& state, float t, float dt)
{
    Derivative a = evaluate(state, t, 0.0f, Derivative());
    Derivative b = evaluate(state, t, dt * 0.5f, a);
    Derivative c = evaluate(state, t, dt * 0.5f, b);
    Derivative d = evaluate(state, t, dt, c);

    const glm::vec3 dxdt = 1.0f / 6.0f * (a.dPosition + 2.0f * (b.dPosition + c.dPosition) + d.dPosition);
    const glm::vec3 dvdt = 1.0f / 6.0f * (a.dVelocity + 2.0f * (b.dVelocity + c.dVelocity) + d.dVelocity);

    state.position = state.position + dxdt * dt;
    state.velocity = state.velocity + dvdt * dt;
}
