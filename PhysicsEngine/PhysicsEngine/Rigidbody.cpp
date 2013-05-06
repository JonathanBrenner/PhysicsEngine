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
    inverseInertiaTensor = glm::inverse(inertiaTensor);
}

void Rigidbody::init()
{
    enabled = false;
	mass = 1;
    
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
    angularVelocity = inverseInertiaTensor * angularMomentum;
    
    // Update Model matrix
    glm::mat4 newModelMatrix = glm::translate(glm::mat4(), state.position) * glm::mat4_cast(orientation);
    gameObject->transform.modelMatrix = newModelMatrix;
    
    glm::vec4 temp(centerOfMass.x, centerOfMass.y, centerOfMass.z, 1);
    temp = gameObject->transform.modelMatrix * temp;
    centerOfMass.x = temp.x;
    centerOfMass.y = temp.y;
    centerOfMass.z = temp.z;
}

void Rigidbody::calcCenterOfMass()
{
    glm::vec3 sum = glm::vec3(0, 0, 0);
    unsigned long size = gameObject->vertices.size();
    
    for (int i = 0; i < size; i++)
    {
        glm::vec3 temp;
        temp.x = gameObject->vertices[i].position[0];
        temp.y = gameObject->vertices[i].position[1];
        temp.z = gameObject->vertices[i].position[2];
        sum += temp;
    }

    centerOfMass = sum * (1.0f / size);
    glm::vec4 temp(centerOfMass.x, centerOfMass.y, centerOfMass.z, 1);
    temp = gameObject->transform.modelMatrix * temp;
    centerOfMass.x = temp.x;
    centerOfMass.y = temp.y;
    centerOfMass.z = temp.z;
    
    for (int i = 0; i < size; i++)
    {
        gameObject->vertices[i].position[0] -= centerOfMass.x;
        gameObject->vertices[i].position[1] -= centerOfMass.y;
        gameObject->vertices[i].position[2] -= centerOfMass.z;
    }
    centerOfMass = glm::vec3(0, 0, 0);
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
    glm::vec3 angularVelocityVector = inverseInertiaTensor * state.angularMomentum;
    state.angularVelocity = glm::quat(0, angularVelocityVector.x, angularVelocityVector.y, angularVelocityVector.z);
    state.orientation = glm::normalize(state.orientation);
    state.spin = state.angularVelocity * state.orientation * Time::deltaTime / 2;
}


