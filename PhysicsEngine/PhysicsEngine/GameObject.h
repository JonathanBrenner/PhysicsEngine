//
//  GameObject.h
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#ifndef __PhysicsEngine__GameObject__
#define __PhysicsEngine__GameObject__

#include <iostream>
#include "Transform.h"
#include "Rigidbody.h"

class GameObject
{
public:
    GameObject();
    GameObject(const GameObject& orig);

    Transform transform;
    Rigidbody rigidbody;

private:

};

#endif /* defined(__PhysicsEngine__GameObject__) */
