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
#include "matrix_transform.hpp"

class Transform
{
public:
    glm::vec3 position;
    
	Transform();

    void translate(float x, float y, float z);
    
    // Rotate using Euler angles
    void rotate(float x, float y, float z);

	glm::mat4 modelMatrix;

private:
};

#endif /* defined(__PhysicsEngine__Transform__) */
