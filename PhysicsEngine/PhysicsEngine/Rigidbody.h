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

class Rigidbody
{
public:
    glm::vec3 velocity;
    float mass = 1;         // kilograms

private:
};

#endif /* defined(__PhysicsEngine__Rigidbody__) */
