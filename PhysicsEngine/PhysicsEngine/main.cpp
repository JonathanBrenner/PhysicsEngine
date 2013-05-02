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
bool CollisionDetection();
void CollisionResponse();


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
    
	GameObject* object1 = new GameObject("container.obj", 1);
	//GameObject* object2 = new GameObject("cuboid.obj", 2);
	gameObjects.push_back(object1);
	//gameObjects.push_back(object2);
    
    Rigidbody rgdbdy = Rigidbody(0.5, 1, 0.5);
    object1->addRigidbody(rgdbdy);
    object1->transform.translate(5, 0, 0);
	//object1->transform.translate(0, 0, -50);
    //object2->transform.translate(10, 0, 0);
	//object1->rigidbody.momentum = glm::vec3(0, 0, -0.5);
    object1->rigidbody.force = glm::vec3(-.01, 0, 0);
    //object1->rigidbody.torque = glm::vec3(0.05, 0.05, -0.05);
    object1->rigidbody.enabled = true;

	//Rigidbody rgbdy2 = Rigidbody();
	//object2->addRigidBody(rgbdy2);
	//object2->transform.translate(-1.0, 0.0, 0.0);
	//object2->rigidbody.enabled = true;

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

//		if(CollisionDetection::intersects(gameObjects[0]->collider, gameObjects[1]->collider))
//		{
//			//std::cout << "Yes\n";
//		}
//		else
//		{
//			//std::cout << "No\n";
//		}

        //Key events
        // Did the user press ESC?
        if( glfwGetKey( GLFW_KEY_ESC ) )
        {
            playing = GL_FALSE;
        }
       
		//Collision
		CollisionDetection();
        
        // Display
        DrawCube();
        glfwSwapBuffers();
    }
}

// I'll do my thing here
bool CollisionDetection()
{
	return true;
}

void CreateCube()
{
	//Create all the things
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->create(shaderProgramID);
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
	if (status == GL_FALSE) {
		int loglen;
		char logbuffer[1000];
		//there's also a corresponding glGetProgramInfoLog function for the linked program object
		glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Shader Compile Error:\n%.*s", loglen, logbuffer);
	} else {
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
