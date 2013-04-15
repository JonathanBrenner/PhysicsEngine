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
	GameObject(std::string pathName, int index);
    GameObject(const GameObject& orig);
	~GameObject();

	void Create(GLuint shaderProgramID1);
	void UpdateModelMatrix();
	void Draw();

	void OnGLError(const char* error_message);
	GLuint LoadShader(const char* filename, GLenum shader_type);

    Transform transform;
    Rigidbody rigidbody;
	std::vector<Vertex> vertices;

private:
	std::vector<GLuint> indices;
	std::vector<UV> uvs;
	std::vector<Normal> normals;

	GLuint modelMatrixUniformLocation;
	GLuint vboID;
	GLuint vaoID;
	GLuint iboID;
	GLuint texID;
	GLuint samplerLoc;
	GLuint shaderProgramID;
	GLuint fragmentShaderID;
	GLuint vertexShaderID;
	GLuint ProjectionMatrixUniformLocation;
	GLuint ViewMatrixUniformLocation;
	GLuint LightMatrixUniformLocation;

	glm::mat4 ViewMatrix;
	glm::mat4 LightMatrix;
	glm::mat4 ProjectionMatrix;
};

#endif /* defined(__PhysicsEngine__GameObject__) */
