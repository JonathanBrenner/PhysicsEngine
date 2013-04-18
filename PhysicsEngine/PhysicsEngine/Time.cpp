//
//  Time.cpp
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/15/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "Time.h"

double Time::time = 0;
double Time::deltaTime = 0;

void Time::update(double currentTime)
{
    deltaTime = currentTime - time;
    time = currentTime;
}