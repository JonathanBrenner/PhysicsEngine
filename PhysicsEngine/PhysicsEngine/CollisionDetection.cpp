//
//  CollisionDetection.cpp
//  PhysicsEngine
//
//  Created by The Great Man Himself on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "CollisionDetection.h"

bool CollisionDetection::intersects(IConvexRegion regioneOne, IConvexRegion regionTwo)
{
	glm::vec3 one(1, 1, 1);
    glm::vec3 s = support(regioneOne, regionTwo, one);
    Simplex simplex;
	simplex.add(s);
 
    glm::vec3 d = -s;

    int maxIterations = 50;

    for (int i = 0; i < maxIterations; i++)
    {
        glm::vec3 a = support(regioneOne, regionTwo, d);

        if (glm::dot(a, d) < 0)
        {
			//std::cout << "fail at " << i << std::endl;
            return false;
        }

        simplex.add(a);
        if (processSimplex(simplex, d))
        {
			//std::cout << "succeed at " << i << std::endl;
			//std::cout << d.x << " " << d.y << " " << d.z;
			//simplex.print();
            return true;
        }
    }
	//std::cout << "default " << std::endl;
    return true;
}

bool CollisionDetection::processSimplex(Simplex &simplex, glm::vec3 &direction)
{
	if (simplex.count() == 2)
    {
        return processLine(simplex, direction);
    }
    else if (simplex.count() == 3)
    {
        return processTriangle(simplex, direction);
    }
    else
    {
        return processTetrehedron(simplex, direction);
    }
}

bool CollisionDetection::processLine(Simplex &simplex, glm::vec3 &direction)
{
	glm::vec3 a = simplex.get(1);
    glm::vec3 b = simplex.get(0);
    glm::vec3 ab = b - a;
    glm::vec3 aO = -a;
 
    if (glm::dot(ab, aO) > 0)
    {
        float dot = glm::dot(ab, aO);
		float angle = (float)acos(dot / (glm::length(ab) * glm::length(aO)));
		direction = glm::cross(glm::cross(ab, aO), ab);
    }
    else
    {
        simplex.remove(b);
        direction = aO;
    }
    return false;
}

bool CollisionDetection::processTriangle(Simplex &simplex, glm::vec3 &direction)
{
	glm::vec3 a = simplex.get(2);
    glm::vec3 b = simplex.get(1);
    glm::vec3 c = simplex.get(0);
    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 abc = glm::cross(ab, ac);
	//glm::vec3 abc = glm::cross(ac, ab);
    glm::vec3 aO = -a;
    glm::vec3 acNormal = glm::cross(abc, ac);
    glm::vec3 abNormal = glm::cross(ab, abc);
 
    if (glm::dot(acNormal, aO) > 0)
    {
        if (glm::dot(ac, aO) > 0)
        {
            simplex.remove(b);
            direction = glm::cross(glm::cross(ac, aO), ac);
        }
        else
        {
            if (glm::dot(ab, aO) > 0)
            {
                simplex.remove(c);
                direction = glm::cross(glm::cross(ab, aO), ab);
            }
            else
            {
                simplex.remove(b);
                simplex.remove(c);
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
                direction = glm::cross(glm::cross(ab, aO), ab);
            }
            else
            {
                simplex.remove(b);
                simplex.remove(c);
                direction = aO;
            }
        }
        else
        {
            if (glm::dot(abc, aO) > 0)
            {
                //direction = glm::cross(glm::cross(abc, aO), abc);
				direction = abc;
            }
            else
            {
                //direction = glm::cross(glm::cross(-abc, aO), -abc);
				direction = -abc;
            }
        }
    }
    return false;
}

bool CollisionDetection::processTetrehedron(Simplex &simplex, glm::vec3 &direction)
{
	glm::vec3 a = simplex.get(3);
    glm::vec3 b = simplex.get(2);
    glm::vec3 c = simplex.get(1);
    glm::vec3 d = simplex.get(0);
    glm::vec3 ac = c - a;
    glm::vec3 ad = d - a;
    glm::vec3 ab = b - a;
    glm::vec3 bc = c - b;
    glm::vec3 bd = d - b;
             
    //glm::vec3 acd = glm::cross(ad, ac);
	glm::vec3 acd = glm::cross(ac, ad);
    glm::vec3 abd = glm::cross(ab, ad);
	//glm::vec3 abd = glm::cross(ad, ab);
    //glm::vec3 abc = glm::cross(ac, ab);
	glm::vec3 abc = glm::cross(ab, ac);
             
    glm::vec3 aO = -a;
 
    if (glm::dot(abc, aO) > 0)
    {
        if (glm::dot(glm::cross(abc, ac), aO) > 0)
        {
            simplex.remove(b);
            simplex.remove(d);
            direction = glm::cross(glm::cross(ac, aO), ac);
        }
        else if (glm::dot(glm::cross(ab, abc), aO) > 0)
        {
            simplex.remove(c);
            simplex.remove(d);
            direction = glm::cross(glm::cross(ab, aO), ab);
        }
        else
        {
            simplex.remove(d);
            direction = abc;
        }
    }
    else if (glm::dot(acd, aO) > 0)
    {
        if (glm::dot(glm::cross(acd, ad), aO) > 0)
        {
            simplex.remove(b);
            simplex.remove(c);
            direction = glm::cross(glm::cross(ad, aO), ad);
        }
        else if (glm::dot(glm::cross(ac, acd), aO) > 0)
        {
            simplex.remove(b);
            simplex.remove(d);
            direction = glm::cross(glm::cross(ac, aO), ac);
        }
        else
        {
            simplex.remove(b);
            direction = acd;
        }
    }
    else if (glm::dot(abd, aO) > 0)
    {
        if (glm::dot(glm::cross(abd, ab), aO) > 0)
        {
            simplex.remove(c);
            simplex.remove(d);
            direction = glm::cross(glm::cross(ab, aO), ab);
        }
        else if (glm::dot(glm::cross(ad, abd), aO) > 0)
        {
            simplex.remove(b);
            simplex.remove(c);
            direction = glm::cross(glm::cross(ad, aO), ad);
        }
        else
        {
            simplex.remove(c);
            direction = abd;
        }
    }
    else
    {
		//std::cout << abd.x << " " << abd.y << " " << abd.z << std::endl;
		//std::cout << acd.x << " " << acd.y << " " << acd.z << std::endl;
		//std::cout << abc.x << " " << abc.y << " " << abc.z << std::endl;
		//std:: cout << a.x << " " << a.y << " " << a.z << std::endl;
		//std:: cout << b.x << " " << b.y << " " << b.z << std::endl;
		//std:: cout << c.x << " " << c.y << " " << c.z << std::endl;
		//std:: cout << d.x << " " << d.y << " " << d.z << std::endl;
        return true;
    }
 
    return false;
}

glm::vec3 CollisionDetection::support(IConvexRegion regionOne, IConvexRegion regionTwo, glm::vec3 direction)
{
	return regionOne.getFurthestPoint(direction) - regionTwo.getFurthestPoint(-direction);
}