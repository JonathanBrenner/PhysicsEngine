//
//  PhysicsVector.h
//  PhysicsEngine
//
//  Created by Jon Snow on 4/7/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#ifndef __PhysicsEngine__PhysicsVector__
#define __PhysicsEngine__PhysicsVector__

#include <iostream>

class PhysicsVector
{
public:
	PhysicsVector();

	void Set(PhysicsVector newVector);

	static PhysicsVector Negate(PhysicsVector a);
	static PhysicsVector DotProduct(PhysicsVector a, PhysicsVector b);
	static PhysicsVector TripleProduct(PhysicsVector a, PhysicsVector b, PhysicsVector c);

    float x;
    float y;
    float z;
    
private:
};

#endif /* defined(__PhysicsEngine__PhysicsVector__) */