//
//  Time.h
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/15/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#ifndef __PhysicsEngine__Time__
#define __PhysicsEngine__Time__

#include <iostream>

class Time
{
public:
    static void update(double currentTime);

    static double time;
    static double deltaTime;
private:
};

#endif /* defined(__PhysicsEngine__Time__) */
