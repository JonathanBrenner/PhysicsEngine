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

/*Simplex::Simplex(std::list<glm::vec3> newVertices)
{
    for (int i = 0; i < newVertices.size(); i++)
    {
        vertices.push_back(newVertices[i]);
    }
}*/

int Simplex::count()
{
    return vertices.size();
}
 
/*glm::vec3 Simplex::this[int i]
{
    return vertices[i];
}*/
       
void Simplex::add(glm::vec3 vertex)
{
    vertices.push_back(vertex);
}
 
void Simplex::remove(glm::vec3 vertex)
{ 
	vertices.remove(vertex);
}