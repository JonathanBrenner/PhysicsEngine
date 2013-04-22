//
//  Simplex.h
//  PhysicsEngine
//
//  Created by Lord Voldemort on 4/7/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#ifndef __PhysicsEngine__Simplex__
#define __PhysicsEngine__Simplex__

#include <iostream>
#include <list>
#include "glm.hpp"

class Simplex
{
public:
    Simplex();
	//Simplex(glm::vec3[] newVertices);

	//glm::vec3 this[int i];

	void add(glm::vec3 vertex);
	void remove(glm::vec3 vertex);

    int count();
    
private:
	std::list<glm::vec3> vertices;
};
#endif /* defined(__PhysicsEngine__Simplex__) */