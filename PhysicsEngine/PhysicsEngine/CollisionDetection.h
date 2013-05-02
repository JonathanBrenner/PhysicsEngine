//
//  CollisionDetection.h
//  PhysicsEngine
//
//  Created by Energizer Bunny on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#ifndef __PhysicsEngine__CollisionDetection__
#define __PhysicsEngine__CollisionDetection__

#include "IConvexRegion.h"
#include "Simplex.h"
#include "math.h"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"

class CollisionDetection
{
public:
	static glm::vec3 findCollisionPoint(Simplex simplex, Simplex A, Simplex B);
	static glm::vec3 intersects(IConvexRegion regioneOne, IConvexRegion regionTwo);

	static bool processSimplex(Simplex &simplex, Simplex &A, Simplex &B, glm::vec3 &direction);
	static bool processLine(Simplex &simplex, Simplex &A, Simplex &B, glm::vec3 &direction);
	static bool processTriangle(Simplex &simplex, Simplex &A, Simplex &B, glm::vec3 &direction);
	static bool processTetrehedron(Simplex &simplex, Simplex &A, Simplex &B, glm::vec3 &direction);

	static glm::vec3 support(IConvexRegion regionOne, IConvexRegion regionTwo, glm::vec3 direction, glm::vec3 &collisionOne, glm::vec3 &collisionTwo);
private:
};
#endif /* defined(__PhysicsEngine__CollisionDetection__) */