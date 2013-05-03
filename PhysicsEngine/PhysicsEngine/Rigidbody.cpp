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
    init();
}

Rigidbody::Rigidbody(float width, float height, float depth)
{
    init();
    this->width = width;
    this->height = height;
    this->depth = depth;
    inertiaTensor[0][0] = mass * (height * height + depth * depth) / 12;
    inertiaTensor[1][1] = mass * (width * width + depth * depth) / 12;
    inertiaTensor[2][2] = mass * (height * height + width * width) / 12;
}

void Rigidbody::init()
{
    enabled = false;
	mass = 1;
    elasticity = 1;
    
    momentum = glm::vec3(0, 0, 0);
    force = glm::vec3(0, 0, 0);
    
    angularMomentum = glm::vec3(0, 0, 0);
    torque = glm::vec3(0, 0, 0);
    orientation = glm::quat(0, 0, 0, 1);

}

void Rigidbody::update()
{
    // RK4 integration
    Rigidbody::State state;
    state.position = gameObject->transform.position;
    state.momentum = momentum;
    state.orientation = orientation;
    state.angularMomentum = angularMomentum;
    
    integrate(state, Time::time, Time::deltaTime);
    
    momentum = state.momentum;
    orientation = state.orientation;
    angularMomentum = state.angularMomentum;
    
    // Update Model matrix
    glm::mat4 newModelMatrix = glm::translate(glm::mat4(), state.position) * glm::mat4_cast(orientation);
    gameObject->transform.modelMatrix = newModelMatrix;
}

void Rigidbody::onCollision(GameObject* other, glm::vec3 collisionPoint)
{
    Rigidbody otherRigidbody = other->rigidbody;
    glm::vec3 radialPosition = collisionPoint - gameObject->transform.position;
    glm::vec3 otherRadialPosition = collisionPoint - other->transform.position;
    
    // Find the normal of the collision (approximating using the vector connecting the center of mass of both objects)
    glm::vec3 collisionNormal = other->transform.position - gameObject->transform.position;
    collisionNormal = glm::normalize(collisionNormal);
    
    // Find the velocity of both objects
    glm::vec3 velocity = momentum / mass;
    glm::vec3 otherVelocity = otherRigidbody.momentum / otherRigidbody.mass;
    glm::vec3 relativeVelocity = otherVelocity - velocity;
    
    glm::mat3 inverseInertiaTensor = glm::inverse(inertiaTensor);
    glm::mat3 otherInverseInertiaTensor = glm::inverse(other->rigidbody.inertiaTensor);
    
    float impulse = glm::dot(-(1 + elasticity) * relativeVelocity, collisionNormal) /
        ((1 / mass) + (1 / otherRigidbody.mass) + glm::dot(glm::cross(inverseInertiaTensor * glm::cross(radialPosition, collisionNormal), radialPosition) +
        glm::cross(otherInverseInertiaTensor * glm::cross(otherRadialPosition, collisionNormal), otherRadialPosition), collisionNormal));
    
    momentum -= impulse * collisionNormal;
    angularMomentum -= impulse * glm::cross(radialPosition, collisionNormal);
}

glm::vec3 Rigidbody::getForce(bool worldCoordinates)
{
    if (worldCoordinates)
    {
        glm::vec4 forceVector4 = glm::vec4(force.x, force.y, force.z, 0);
        forceVector4 = gameObject->transform.modelMatrix * forceVector4;
        return glm::vec3(forceVector4.x, forceVector4.y, forceVector4.z);
    }
    else
    {
        return force;
    }
}

Rigidbody::Derivative Rigidbody::evaluate(Rigidbody::State& state, float t, float dt, const Rigidbody::Derivative &derivative)
{
    state.position = state.position + derivative.velocity * dt;
    state.momentum = state.momentum + force * dt;
    state.orientation = state.orientation + derivative.spin * dt;
    state.angularMomentum = state.angularMomentum + torque * dt;
    recalculate(state);
    
    Rigidbody::Derivative output;
    output.velocity = state.velocity;
    output.spin = state.spin;
    
    return output;
}

void Rigidbody::integrate(State& state, float t, float dt)
{
    Derivative a = evaluate(state, t, 0.0f, Derivative());
    Derivative b = evaluate(state, t, dt * 0.5f, a);
    Derivative c = evaluate(state, t, dt * 0.5f, b);
    Derivative d = evaluate(state, t, dt, c);

    state.position = state.position + 1.0f / 6.0f * (a.velocity + 2.0f * (b.velocity + c.velocity) + d.velocity) * dt;
    state.momentum = state.momentum + force * dt;
    state.orientation = state.orientation + 1.0f / 6.0f * (a.spin + 2.0f * (b.spin + c.spin) + d.spin) * dt;
    state.angularMomentum = state.angularMomentum + torque * dt;
    
    recalculate(state);
}

void Rigidbody::recalculate(State& state)
{
    state.velocity = state.momentum / mass;    
    glm::vec3 angularVelocityVector = state.angularMomentum * glm::inverse(inertiaTensor);
    state.angularVelocity = glm::quat(0, angularVelocityVector.x, angularVelocityVector.y, angularVelocityVector.z);
    state.orientation = glm::normalize(state.orientation);
    state.spin = state.angularVelocity * state.orientation * Time::deltaTime / 2;
}


