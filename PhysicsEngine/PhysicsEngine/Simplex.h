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
#include <vector>
#include "glm.hpp"

class Simplex
{
public:
    Simplex();
	Simplex(std::vector<glm::vec3> newVertices);
	Simplex(glm::vec3 copyVertex);

	void add(glm::vec3 vertex);
	void remove(glm::vec3 vertex);
	void print();

	glm::vec3 get(int i);
    int count();
    
private:
	std::vector<glm::vec3> vertices;
};
#endif /* defined(__PhysicsEngine__Simplex__) */