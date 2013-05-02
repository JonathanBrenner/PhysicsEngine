//
//  IConvexRegion.cpp
//  PhysicsEngine
//
//  Created by Energizer Bunny's Long Lost Evil Twin on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "IConvexRegion.h"
#include "GameObject.h"

IConvexRegion::IConvexRegion()
{
}

IConvexRegion::IConvexRegion(glm::vec3 newCenter, float newRadius)
{
    center = newCenter;
    radius = newRadius;
}
 
glm::vec3 IConvexRegion::getFurthestPoint(glm::vec3 direction)
{
	glm::vec4 temp(parent->vertices[0].position[0], parent->vertices[0].position[1], parent->vertices[0].position[2], 1);
	glm::mat4x4 model = parent->transform.modelMatrix;
	temp = model * temp;

	glm::vec4 newDirection(direction.x, direction.y, direction.z, 0);

	float max = glm::dot(temp, newDirection);
    int index = 0;
    for (int i = 1; i < parent->vertices.size(); i++)
    {
		temp.x = parent->vertices[i].position[0];
		temp.y = parent->vertices[i].position[1];
		temp.z = parent->vertices[i].position[2];
		temp.w = 1;
		temp = model * temp;

        float dot = glm::dot(temp, newDirection);
        if (dot > max)
        {
            max = dot;
            index = i;
        }
    }
	temp.x = parent->vertices[index].position[0];
	temp.y = parent->vertices[index].position[1];
	temp.z = parent->vertices[index].position[2];
	temp.w = 1;
	temp = model * temp;

	glm::vec3 answer(temp.x, temp.y, temp.z);
    return answer;
}