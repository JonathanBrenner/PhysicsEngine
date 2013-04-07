//
//  Transform.cpp
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "Transform.h"

void Transform::translate(float x, float y, float z)
{
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
}

void Transform::rotate(float x, float y, float z)
{
    modelMatrix = glm::rotate(modelMatrix, z, glm::vec3(0, 0, z));
    modelMatrix = glm::rotate(modelMatrix, x, glm::vec3(x, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, y, glm::vec3(0, y, 0));
}