//
//  IConvexRegion.cpp
//  PhysicsEngine
//
//  Created by Energizer Bunny's Pet Rabbit George on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#ifndef __PhysicsEngine__IConvexRegion__
#define __PhysicsEngine__IConvexRegion__

#include "glm.hpp"

class GameObject;

class IConvexRegion
{
public:
	IConvexRegion();
	IConvexRegion(glm::vec3 newCenter, float newRadius);

	glm::vec3 getFurthestPoint(glm::vec3 direction);

	glm::vec3 center;
    float radius;

	GameObject* parent;
};
#endif /* defined(__PhysicsEngine__IConvexRegion__) */
