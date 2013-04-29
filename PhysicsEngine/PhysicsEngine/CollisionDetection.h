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

class CollisionDetection
{
public:
	static bool intersects(IConvexRegion regioneOne, IConvexRegion regionTwo);

	static bool processSimplex(Simplex &simplex, glm::vec3 &direction);
	static bool processLine(Simplex &simplex, glm::vec3 &direction);
	static bool processTriangle(Simplex &simplex, glm::vec3 &direction);
	static bool processTetrehedron(Simplex &simplex, glm::vec3 &direction);

	static glm::vec3 support(IConvexRegion regionOne, IConvexRegion regionTwo, glm::vec3 direction);
private:
};
#endif /* defined(__PhysicsEngine__CollisionDetection__) */