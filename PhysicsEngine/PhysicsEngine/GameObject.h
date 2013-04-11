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
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Transform.h"
#include "Rigidbody.h"
#include "Vertex.h"
#include "UV.h"
#include "Normal.h"
#include <GL/glew.h> //MUST come before GLFW!
#include <GL/glfw.h>
#include "glm.hpp" //feel free to use your own math library!
#include "matrix_transform.hpp"
#include "type_ptr.hpp"

class GameObject
{
public:
    GameObject();
	GameObject(std::string pathName);
    GameObject(const GameObject& orig);

	void Create(GLuint shaderProgramID);
	void UpdateModelMatrix();
	void Draw();

    Transform transform;
    Rigidbody rigidbody;

	void OnGLError(const char* error_message);
	std::vector<Vertex> vertices;
private:
	std::vector<GLuint> indices;
	std::vector<UV> uvs;
	std::vector<Normal> normals;

	GLuint modelMatrixUniformLocation;
	GLuint vboID;
	GLuint iboID;
	GLuint texID;
};

#endif /* defined(__PhysicsEngine__GameObject__) */
