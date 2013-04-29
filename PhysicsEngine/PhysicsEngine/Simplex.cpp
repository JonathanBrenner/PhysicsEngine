//
//  Simplex.cpp
//  PhysicsEngine
//
//  Created by The Panda Man on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "Simplex.h"

Simplex::Simplex()
{
}

Simplex::Simplex(std::vector<glm::vec3> newVertices)
{
    for (int i = 0; i < newVertices.size(); i++)
    {
        vertices.push_back(newVertices[i]);
    }
}

Simplex::Simplex(glm::vec3 newVertex)
{
	vertices.clear();
	vertices.push_back(newVertex);
}

int Simplex::count()
{
    return vertices.size();
}
 
glm::vec3 Simplex::get(int i)
{
    return vertices[i];
}
       
void Simplex::add(glm::vec3 vertex)
{
	//std::cout << "adding " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    vertices.push_back(vertex);
}
 
void Simplex::remove(glm::vec3 vertex)
{
	//std::cout << "removing" << std::endl;
	for(int i = 0; i < vertices.size(); i++)
	{
		if(vertices[i].x == vertex.x && vertices[i].y == vertex.y && vertices[i].z == vertex.z)
		{
			//std::cout << "erasing " << std::endl;
			vertices.erase(vertices.begin() + i, vertices.begin() + i + 1);
			break;
		}
	}
}

void Simplex::print()
{
	for(int i = 0; i < vertices.size(); i++)
	{
		std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
	}
}