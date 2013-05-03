//
//  CollisionDetection.cpp
//  PhysicsEngine
//
//  Created by The Great Man Himself on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "CollisionDetection.h"

glm::vec3 CollisionDetection::findCollisionPoint(Simplex simplex, Simplex A, Simplex B)
{
	if(simplex.count() == 4)
	{
		glm::mat4x4 bottom;
		bottom[0][0] = 1;
		bottom[0][1] = simplex.get(0).x;
		bottom[0][2] = simplex.get(0).y;
		bottom[0][3] = simplex.get(0).z;
		
		bottom[1][0] = 1;
		bottom[1][1] = simplex.get(1).x;
		bottom[1][2] = simplex.get(1).y;
		bottom[1][3] = simplex.get(1).z;
		
		bottom[2][0] = 1;
		bottom[2][1] = simplex.get(2).x;
		bottom[2][2] = simplex.get(2).y;
		bottom[2][3] = simplex.get(2).z;
		
		bottom[3][0] = 1;
		bottom[3][1] = simplex.get(3).x;
		bottom[3][2] = simplex.get(3).y;
		bottom[3][3] = simplex.get(3).z;

		float det = glm::determinant(bottom);

		glm::mat4x4 one = bottom;
		one[0][1] = 0;
		one[0][2] = 0;
		one[0][3] = 0;
		float lambda1 = glm::determinant(one) / det;

		glm::mat4x4 two = bottom;
		two[1][1] = 0;
		two[1][2] = 0;
		two[1][3] = 0;
		float lambda2 = glm::determinant(two) / det;

		glm::mat4x4 three = bottom;
		three[2][1] = 0;
		three[2][2] = 0;
		three[2][3] = 0;
		float lambda3 = glm::determinant(three) / det;

		glm::mat4x4 four = bottom;
		four[3][1] = 0;
		four[3][2] = 0;
		four[3][3] = 0;
		float lambda4 = glm::determinant(four) / det;

		glm::vec3 collision;
		collision = lambda1 * A.get(0) + lambda2 * A.get(1) + lambda3 * A.get(2) + lambda4 * A.get(3);
		return collision;
	}
	else if (simplex.count() == 3)
	{
		glm::mat3x3 bottom;
		bottom[0][0] = 1;
		bottom[0][1] = simplex.get(0).x;
		bottom[0][2] = simplex.get(0).y;
		
		bottom[1][0] = 1;
		bottom[1][1] = simplex.get(1).x;
		bottom[1][2] = simplex.get(1).y;
		
		bottom[2][0] = 1;
		bottom[2][1] = simplex.get(2).x;
		bottom[2][2] = simplex.get(2).y;

		float det = glm::determinant(bottom);

		glm::mat3x3 one = bottom;
		one[0][1] = 0;
		one[0][2] = 0;
		float lambda1 = glm::determinant(one) / det;

		glm::mat3x3 two = bottom;
		two[1][1] = 0;
		two[1][2] = 0;
		float lambda2 = glm::determinant(two) / det;

		glm::mat3x3 three = bottom;
		three[2][1] = 0;
		three[2][2] = 0;
		float lambda3 = glm::determinant(three) / det;

		glm::vec3 collision;
		collision = lambda1 * A.get(0) + lambda2 * A.get(1) + lambda3 * A.get(2);
		//std::cout << "SIMPLEX SIZE OF THREE " << std::endl;
		return collision;
	}
	else if(simplex.count() == 2)
	{
		glm::mat2x2 bottom;
		bottom[0][0] = 1;
		bottom[0][1] = simplex.get(0).x;
		
		bottom[1][0] = 1;
		bottom[1][1] = simplex.get(1).x;

		float det = glm::determinant(bottom);

		glm::mat2x2 one = bottom;
		one[0][1] = 0;
		float lambda1 = glm::determinant(one) / det;

		glm::mat2x2 two = bottom;
		two[1][1] = 0;
		float lambda2 = glm::determinant(two) / det;

		glm::vec3 collision;
		collision = lambda1 * A.get(0) + lambda2 * A.get(1);
		//std::cout << "SIMPLEX SIZE OF TWO " << std::endl;
		return collision;
	}

	glm::vec3 zero(0,0,0);
	return zero;
}

glm::vec3 CollisionDetection::intersects(IConvexRegion regioneOne, IConvexRegion regionTwo)
{
	glm::vec3 one(1, 1, 1);
	glm::vec3 collisionOne;
	glm::vec3 collisionTwo;
    glm::vec3 s = support(regioneOne, regionTwo, one, collisionOne, collisionTwo);
    Simplex simplex;
	Simplex A;
	Simplex B;
	simplex.add(s);
	A.add(collisionOne);
	B.add(collisionTwo);
 
    glm::vec3 d = -s;

    int maxIterations = 50;

    for (int i = 0; i < maxIterations; i++)
	{
        glm::vec3 a = support(regioneOne, regionTwo, d, collisionOne, collisionTwo);

        if (glm::dot(a, d) < 0)
        {
			glm::vec3 zero;
			zero.x = zero.null;
			zero.y = zero.null;
			zero.z = zero.null;
			return zero;
        }

        simplex.add(a);
		A.add(collisionOne);
		B.add(collisionTwo);

        if (processSimplex(simplex, A, B, d))
        {
			return CollisionDetection::findCollisionPoint(simplex, A, B);
           // return true;
        }
    }
	return CollisionDetection::findCollisionPoint(simplex, A, B);
    //return true;
}

bool CollisionDetection::processSimplex(Simplex &simplex, Simplex &A, Simplex &B, glm::vec3 &direction)
{
	if (simplex.count() == 2)
    {
        return processLine(simplex, A, B, direction);
    }
    else if (simplex.count() == 3)
    {
        return processTriangle(simplex, A, B, direction);
    }
    else
    {
        return processTetrehedron(simplex, A, B, direction);
    }
}

bool CollisionDetection::processLine(Simplex &simplex, Simplex &A, Simplex&B, glm::vec3 &direction)
{
	glm::vec3 a = simplex.get(1);
    glm::vec3 b = simplex.get(0);
    glm::vec3 ab = b - a;
    glm::vec3 aO = -a;
 
    if (glm::dot(ab, aO) > 0)
    {
		direction = glm::cross(glm::cross(ab, aO), ab);
    }
    else
    {
        simplex.remove(b);
		A.remove(A.get(0));
		B.remove(B.get(0));
        direction = aO;
    }
    return false;
}

bool CollisionDetection::processTriangle(Simplex &simplex, Simplex &A, Simplex &B, glm::vec3 &direction)
{
	glm::vec3 a = simplex.get(2);
    glm::vec3 b = simplex.get(1);
    glm::vec3 c = simplex.get(0);
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 abc = glm::cross(ab, ac);
    glm::vec3 aO = -a;
    glm::vec3 acNormal = glm::cross(abc, ac);
    glm::vec3 abNormal = glm::cross(ab, abc);
 
    if (glm::dot(acNormal, aO) > 0)
    {
        if (glm::dot(ac, aO) > 0)
        {
            simplex.remove(b);
			A.remove(A.get(1));
			B.remove(B.get(1));
            direction = glm::cross(glm::cross(ac, aO), ac);
        }
        else
        {
            if (glm::dot(ab, aO) > 0)
            {
                simplex.remove(c);
				A.remove(A.get(0));
				B.remove(B.get(0));
                direction = glm::cross(glm::cross(ab, aO), ab);
            }
            else
            {
                simplex.remove(b);
				A.remove(A.get(1));
				B.remove(B.get(1));
                simplex.remove(c);
				A.remove(A.get(0));
				B.remove(B.get(0));
                direction = aO;
            }
        }
    }
    else
    {
        if (glm::dot(abNormal, aO) > 0)
        {
            if (glm::dot(ab, aO) > 0)
            {
                simplex.remove(c);
				A.remove(A.get(0));
				B.remove(B.get(0));
                direction = glm::cross(glm::cross(ab, aO), ab);
            }
            else
            {
                simplex.remove(b);
				A.remove(A.get(1));
				B.remove(B.get(1));
                simplex.remove(c);
				A.remove(A.get(0));
				B.remove(B.get(0));
                direction = aO;
            }
        }
        else
        {
            if (glm::dot(abc, aO) > 0)
            {
				direction = abc;
            }
            else
            {
				direction = -abc;
            }
        }
    }
    return false;
}

bool CollisionDetection::processTetrehedron(Simplex &simplex, Simplex &A, Simplex &B, glm::vec3 &direction)
{
	glm::vec3 a = simplex.get(3);
    glm::vec3 b = simplex.get(2);
    glm::vec3 c = simplex.get(1);
    glm::vec3 d = simplex.get(0);
    glm::vec3 ac = c - a;
    glm::vec3 ad = d - a;
    glm::vec3 ab = b - a;
    //glm::vec3 bc = c - b;
    //glm::vec3 bd = d - b;
             
	glm::vec3 acd = glm::cross(ac, ad);
    glm::vec3 abd = glm::cross(ab, ad);
	glm::vec3 abc = glm::cross(ab, ac);
             
    glm::vec3 aO = -a;
 
    if (glm::dot(abc, aO) > 0)
    {
        if (glm::dot(glm::cross(abc, ac), aO) > 0)
        {
            simplex.remove(b);
			A.remove(A.get(2));
			B.remove(B.get(2));
            simplex.remove(d);
			A.remove(A.get(0));
			B.remove(B.get(0));
            direction = glm::cross(glm::cross(ac, aO), ac);
        }
        else if (glm::dot(glm::cross(ab, abc), aO) > 0)
        {
            simplex.remove(c);
			A.remove(A.get(1));
			B.remove(B.get(1));
            simplex.remove(d);
			A.remove(A.get(0));
			B.remove(B.get(0));
            direction = glm::cross(glm::cross(ab, aO), ab);
        }
        else
        {
            simplex.remove(d);
			A.remove(A.get(0));
			B.remove(B.get(0));
            direction = abc;
        }
    }
    else if (glm::dot(acd, aO) > 0)
    {
        if (glm::dot(glm::cross(acd, ad), aO) > 0)
        {
            simplex.remove(b);
			A.remove(A.get(2));
			B.remove(B.get(2));
            simplex.remove(c);
			A.remove(A.get(1));
			B.remove(B.get(1));
            direction = glm::cross(glm::cross(ad, aO), ad);
        }
        else if (glm::dot(glm::cross(ac, acd), aO) > 0)
        {
            simplex.remove(b);
			A.remove(A.get(2));
			B.remove(B.get(2));
            simplex.remove(d);
			A.remove(A.get(0));
			B.remove(B.get(0));
            direction = glm::cross(glm::cross(ac, aO), ac);
        }
        else
        {
            simplex.remove(b);
			A.remove(A.get(2));
			B.remove(B.get(2));
            direction = acd;
        }
    }
    else if (glm::dot(abd, aO) > 0)
    {
        if (glm::dot(glm::cross(abd, ad), aO) > 0)
        {
            simplex.remove(c);
			A.remove(A.get(1));
			B.remove(B.get(1));
            simplex.remove(b);
			A.remove(A.get(2));
			B.remove(B.get(2));
            direction = glm::cross(glm::cross(ad, aO), ad);
        }
        else if (glm::dot(glm::cross(ab, abd), aO) > 0)
        {
            simplex.remove(d);
			A.remove(A.get(0));
			B.remove(B.get(0));
            simplex.remove(c);
			A.remove(A.get(1));
			B.remove(B.get(1));
            direction = glm::cross(glm::cross(ab, aO), ab);
        }
        else
        {
            simplex.remove(c);
			A.remove(A.get(1));
			B.remove(B.get(1));
            direction = abd;
        }
    }
    else
    {
        return true;
    }
 
    return false;
}

glm::vec3 CollisionDetection::support(IConvexRegion regionOne, IConvexRegion regionTwo, glm::vec3 direction, glm::vec3 &collisionOne, glm::vec3 &collisionTwo)
{
	collisionOne = regionOne.getFurthestPoint(direction);
	collisionTwo = regionTwo.getFurthestPoint(-direction);
	return collisionOne - collisionTwo;
}