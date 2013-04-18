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
#include "PhysicsVector.h"

class Simplex
{
public:
    Simplex();

    void Add(PhysicsVector point);
    void Remove(PhysicsVector point);
    
    PhysicsVector GetLast();
    PhysicsVector GetA();
    PhysicsVector GetB();
    
    int Size();
    
private:
    std::vector<PhysicsVector> vertices;
};
#endif /* defined(__PhysicsEngine__Simplex__) */