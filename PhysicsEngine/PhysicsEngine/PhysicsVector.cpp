//
//  PhysicsVector.cpp
//  PhysicsEngine
//
//  Created by Harry Potter on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "PhysicsVector.h"

PhysicsVector::PhysicsVector()
{
    x = 0;
    y = 0;
    z = 0;
}

void PhysicsVector::Set(PhysicsVector newVector)
{
	x = newVector.x;
	y = newVector.y;
	z = newVector.z;
}

PhysicsVector PhysicsVector::Negate(PhysicsVector a)
{
	PhysicsVector tempVector;
	tempVector.x = a.x * -1;
	tempVector.y = a.y * -1;
	tempVector.z = a.z * -1;
	return tempVector;
}
PhysicsVector PhysicsVector::DotProduct(PhysicsVector a, PhysicsVector b)
{
	PhysicsVector tempVector;
	return tempVector;
}

PhysicsVector PhysicsVector::TripleProduct(PhysicsVector a, PhysicsVector b, PhysicsVector c)
{
	PhysicsVector tempVector;
	return tempVector;
}