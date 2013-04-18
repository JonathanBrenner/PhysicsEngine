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

void Simplex::Add(PhysicsVector point)
{
    vertices.push_back(point);
}

void Simplex::Remove(PhysicsVector point)
{
    for(int i = 0; i < vertices.size(); i++)
    {
        if(vertices[i].x == point.x && vertices[i].y == point.y && vertices[i].z == point.z)
        {
            vertices.erase(vertices.begin() + i - 1 , vertices.begin() + i);
        }
    }
}

PhysicsVector Simplex::GetLast()
{
    return vertices[vertices.size() - 1];
}

PhysicsVector Simplex::GetA()
{
    return vertices[0];
}

PhysicsVector Simplex::GetB()
{
    return vertices[1];
}

int Simplex::Size()
{
    return vertices.size();
}