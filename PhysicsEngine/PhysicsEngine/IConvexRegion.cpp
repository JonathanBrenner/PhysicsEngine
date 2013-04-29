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
	/*glm::vec3 zero;
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
    if (direction != zero)
    {
		direction = glm::normalize(direction);
    }
    return center + radius * direction;*/
	glm::vec3 temp;
	temp.x = parent->vertices[0].position[0] + parent->transform.position.x;
	temp.y = parent->vertices[0].position[1] + parent->transform.position.y;
	temp.z = parent->vertices[0].position[2] + parent->transform.position.z;

	float max = glm::dot(temp, direction);
    int index = 0;
    for (int i = 1; i < parent->vertices.size(); i++)
    {
		temp.x = parent->vertices[i].position[0] + parent->transform.position.x;
		temp.y = parent->vertices[i].position[1] + parent->transform.position.y;
		temp.z = parent->vertices[i].position[2] + parent->transform.position.z;
        float dot = glm::dot(temp, direction);
        if (dot > max)
        {
            max = dot;
            index = i;
        }
    }
	temp.x = parent->vertices[index].position[0] + parent->transform.position.x;
	temp.y = parent->vertices[index].position[1] + parent->transform.position.y;
	temp.z = parent->vertices[index].position[2] + parent->transform.position.z;
	//std::cout << temp.x << " " << temp.y << " " << temp.z << std::endl;
    return temp;
}