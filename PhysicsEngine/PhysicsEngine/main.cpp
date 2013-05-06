//
//  main.cpp
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/3/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h> //MUST come before GLFW!
#include <GL/glfw.h>

#include "glm.hpp" //feel free to use your own math library!
#include "matrix_transform.hpp"
#include "type_ptr.hpp"
#include "GameObject.h"
#include "Time.h"
#include "Simplex.h"
#include "CollisionDetection.h"

int WindowWidth = 800, WindowHeight = 600;

//specify the "camera"
glm::mat4 ViewMatrix = glm::mat4(1.0f);
//specify the light source
glm::mat4 LightMatrix = glm::mat4(1.0f);
//specify the matrix to warp points appropriately for a perspective view
glm::mat4 ProjectionMatrix = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.f); //FOV, aspect ratio, near, far

static const double PI = 3.14159265358979323846;

GLuint
    VaoID = 0,
	TimeLocation,
	samplerLoc,
	BufferIds[3] = { 0 },
	ShaderIds[3] = { 0 },
    texID,
	shaderProgramID = 0,
	vertexShaderID = 0,
	fragmentShaderID = 0;
	

float CubeRotation = 0;

std::vector<GameObject*> gameObjects;

void Initialize(int, char*[]);
void InitObject(void);
void InitWindow(void);
void CreateCube(void);
void DrawCube(void);
void game_loop(void);
void checkShader(GLuint);
void OnGLError(const char*);
GLuint LoadShader(const char*, GLenum);
void CollisionDetection();
void CollisionResponse(GameObject& a, GameObject& b, glm::vec3 point);

int main(int argc, char* argv[])
{
	Initialize(argc, argv);
    game_loop();
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	InitWindow();
	OnGLError("Init window");

	glewExperimental = GL_TRUE;
	
    GlewInitResult = glewInit();
    OnGLError("GLEW init");

	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}
	OnGLError("GLEW string");
	
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);
	OnGLError("Get string");
	
    fprintf(
		stdout,
		"INFO: GLEW Version: %s\n",
		glewGetString(GLEW_VERSION) 
	);
	
	//"Global" OpenGL parameters, at least for this example

    //set background color to black, so that clearing the window is equivelent to Pygame's screen.fill((0,0,0))
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	OnGLError("Clear color");
	//do we want depth testing? Usually yes, unless you have a reason not to
	glEnable(GL_DEPTH_TEST);
	OnGLError("Depth test");
	//now that depth testing is on, which technique will we use to determine how to draw pixels based on their z-position?
	//GL_LESS means that if the pixel is closer to the camera than one that's already been drawn, then draw it. What we usually want
	glDepthFunc(GL_LESS);
	OnGLError("Depth Func");
	//these three commands sets OpenGL to not draw the "back" of triangles. Speeds things up!
	glEnable(GL_CULL_FACE);
	OnGLError("Enable cull face");
	glCullFace(GL_BACK);
	OnGLError("Cull face");
	glFrontFace(GL_CCW);
	OnGLError("Front face");
    
	GameObject* object1 = new GameObject("ugly_cylinder.obj", 1);
	GameObject* object2 = new GameObject("ugly_cube.obj", 2);
    //GameObject* object3 = new GameObject("wtf.obj", 3);
	gameObjects.push_back(object1);
	gameObjects.push_back(object2);
    //gameObjects.push_back(object3);
    
    Rigidbody rigidbody1 = Rigidbody(0.5, 1, 0.5);
    object1->addRigidbody(rigidbody1);
    object1->rigidbody.enabled = true;
    //object1->transform.translate(5, 0, 0);
    //object1->rigidbody.momentum = glm::vec3(-0.2, 0, 0);
    
    Rigidbody rigidbody2 = Rigidbody(1, 1, 1);
    object2->addRigidbody(rigidbody2);
    object2->rigidbody.enabled = true;
    object2->transform.translate(-3, 0, 0);
    object2->rigidbody.momentum = glm::vec3(0.3, 0, 0);
    
//    Rigidbody rigidbody3 = Rigidbody(1, 1, 1);
//    object3->addRigidbody(rigidbody3);
//    object3->rigidbody.enabled = true;
//    object3->transform.translate(0, 4.5, 0);
//    object3->rigidbody.momentum = glm::vec3(0, -0.2, 0);

    CreateCube();
}

void InitWindow(void)
{
	// Initialize GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }
    
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	if( !glfwOpenWindow( WindowWidth, WindowHeight, 0,0,0,0, 16,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    
    fprintf(stderr, "GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    glfwEnable(GLFW_STICKY_KEYS);
}

void game_loop(void)
{
    int playing;
    playing = GL_TRUE;
    while( playing && glfwGetWindowParam( GLFW_OPENED ) )
    {
        // Update our time and delta time using GLFW
        Time::update(glfwGetTime());
        
        for (int i = 0; i < gameObjects.size(); i++)
        {
			gameObjects[i]->update();
        }

		CollisionDetection();

        //Key events
        // Did the user press ESC?
        if( glfwGetKey( GLFW_KEY_ESC ) )
        {
            playing = GL_FALSE;
        }

        // Display
        DrawCube();
        glfwSwapBuffers();
    }
}

// Check collisions against all of the objects
void CollisionDetection()
{
	for(int i = 0; i < gameObjects.size(); i++)
	{
		for(int j = i + 1; j < gameObjects.size(); j++)
		{
			glm::vec3 answer = CollisionDetection::intersects(gameObjects[i]->collider, gameObjects[j]->collider);
			if(answer.x != answer.null)
			{
                CollisionResponse(*gameObjects[i], *gameObjects[j], answer);
			}
		}
	}
}

void CollisionResponse(GameObject& a, GameObject& b, glm::vec3 point)
{
    float elasticity = 1;

    glm::vec3 radialPositionA = point - a.rigidbody.centerOfMass;
    glm::vec3 radialPositionB = point - b.rigidbody.centerOfMass;
    
    // Find the normal of the collision (approximating using the vector connecting the center of mass of both objects)
    glm::vec3 normal = a.rigidbody.centerOfMass - b.rigidbody.centerOfMass;
    normal = glm::normalize(normal);
    
    // Find the velocity of both objects
    glm::vec3 pointVelocityA = a.rigidbody.momentum / a.rigidbody.mass + glm::cross(a.rigidbody.angularVelocity, radialPositionA);
    glm::vec3 pointVelocityB = b.rigidbody.momentum / b.rigidbody.mass + glm::cross(b.rigidbody.angularVelocity, radialPositionB);
    
    glm::vec3 relativeVelocity = pointVelocityA - pointVelocityB;
    
    float impulse = -(1 + elasticity) * glm::dot(relativeVelocity, normal) /
        (glm::dot(normal, normal * ((1 / a.rigidbody.mass) + (1 / b.rigidbody.mass))) +
        glm::dot(glm::cross(a.rigidbody.inverseInertiaTensor * glm::cross(radialPositionA, normal), radialPositionA) +
        glm::cross(b.rigidbody.inverseInertiaTensor * glm::cross(radialPositionB, normal), radialPositionB), normal));
    
    //std::cout << "impulse: " << impulse << " angular momentum: " << glm::cross(radialPositionA, normal).x << " " << glm::cross(radialPositionA, normal).y << " " << glm::cross(radialPositionA, normal).z << " relative velocity: " << relativeVelocity.x << " " << relativeVelocity.y << " " << relativeVelocity.z << "\n";
    
    a.rigidbody.momentum += impulse * normal;
    a.rigidbody.angularMomentum += impulse * a.rigidbody.inverseInertiaTensor * glm::cross(radialPositionA, normal);
    
    b.rigidbody.momentum -= impulse * normal;
    b.rigidbody.angularMomentum -= impulse * b.rigidbody.inverseInertiaTensor * glm::cross(radialPositionB, normal);
}

void CreateCube()
{
	//Create all the things
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->create();
	}
}

void DrawCube(void)
{    
    //clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	//Update all the things
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->updateModelMatrix();
	}

	//Draw all the things
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->draw();
	}
}


void checkShader(GLuint shader)
{
	GLint status;
	//lGetShaderiv gets a particular parameter of the shader
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
    {
		int loglen;
		char logbuffer[1000];
		//there's also a corresponding glGetProgramInfoLog function for the linked program object
		glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Shader Compile Error:\n%.*s", loglen, logbuffer);
	}
    else
    {
		int loglen;
		char logbuffer[1000];
		glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Shader Compile OK:\n%.*s", loglen, logbuffer);
	}
}

GLuint LoadShader(const char* filename, GLenum shader_type)
{
	GLuint shader_id = 0;
	FILE* file;
	long file_size = -1;
	GLchar* glsl_source;
    fprintf(stderr, "name: %s\n",filename);

	if (NULL != (file = fopen(filename, "rb")) &&
		0 == fseek(file, 0, SEEK_END) &&
		-1 != (file_size = ftell(file)))
	{
		rewind(file);
		
		if (NULL != (glsl_source = (GLchar*)malloc(file_size + 1)))
		{
			if (file_size == (long)fread(glsl_source, sizeof(GLchar), file_size, file))
			{
				glsl_source[file_size] = '\0';
                const GLchar* glsl_source_c = glsl_source;
				fprintf(stderr, "Source: %s\n", glsl_source_c);

				if (0 != (shader_id = glCreateShader(shader_type)))
				{
					glShaderSource(shader_id, 1, &glsl_source_c, NULL);
					glCompileShader(shader_id);
					OnGLError("Could not compile a shader");
				}
				else
					fprintf(stderr, "ERROR: Could not create a shader.\n");
			}
			else
				fprintf(stderr, "ERROR: Could not read file %s\n", filename);

			free(glsl_source);
		}
		else
			fprintf(stderr, "ERROR: Could not allocate %li bytes.\n", file_size);

		fclose(file);
	}
	else
		fprintf(stderr, "ERROR: Could not open file %s\n", filename);

	return shader_id;
}

void OnGLError(const char* error_message)
{
	const GLenum ErrorValue = glGetError();

	if (ErrorValue != GL_NO_ERROR)
	{
		const char* APPEND_DETAIL_STRING = ": %s\n";
		const size_t APPEND_LENGTH = strlen(APPEND_DETAIL_STRING) + 1;
		const size_t message_length = strlen(error_message);
		char* display_message = (char*)malloc(message_length + APPEND_LENGTH);

		memcpy(display_message, error_message, message_length);
		memcpy(&display_message[message_length], APPEND_DETAIL_STRING, APPEND_LENGTH);

		fprintf(stderr, display_message, gluErrorString(ErrorValue));

		free(display_message);
	}
}
