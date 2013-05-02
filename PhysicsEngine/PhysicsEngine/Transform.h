//
//  Transform.h
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#ifndef __PhysicsEngine__Transform__
#define __PhysicsEngine__Transform__

#include <iostream>
#include "glm.hpp"
#include "quaternion.hpp"
#include "matrix_transform.hpp"

class Transform
{
public:
    glm::vec3 position;
    glm::mat4 modelMatrix;

	Transform();

    void translate(float x, float y, float z);
    void translate(glm::vec3);
    // Rotate using Euler angles
    void rotate(float x, float y, float z);
    void rotate(glm::quat quaternion);
    void update();

private:
};

#endif /* defined(__PhysicsEngine__Transform__) */
