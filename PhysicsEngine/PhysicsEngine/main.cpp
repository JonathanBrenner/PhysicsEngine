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
#include <time.h>
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

int WindowWidth = 800, WindowHeight = 450;

//specify the "camera"
glm::mat4 ViewMatrix = glm::mat4(1.0f);
//specify the light source
glm::mat4 LightMatrix = glm::mat4(1.0f);
//specify the matrix to warp points appropriately for a perspective view
glm::mat4 ProjectionMatrix = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.f); //FOV, aspect ratio, near, far

static const double PI = 3.14159265358979323846;

GLuint
	//ProjectionMatrixUniformLocation,
	//ViewMatrixUniformLocation,
	//LightMatrixUniformLocation,
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
double LastTime = 0, Now = 0; //Not floats! We may need the resolution!

std::vector<GameObject*> gameObjects;

void Initialize(int, char*[]);
void InitObject(void);
void InitWindow(void);
void CreateCube(void);
void DrawCube(void);
//void DestroyCube(void);
void game_loop(void);
void checkShader(GLuint);
void OnGLError(const char*);
GLuint LoadShader(const char*, GLenum);


int main(int argc, char* argv[])
{
	Initialize(argc, argv);
    game_loop();
    //DestroyCube();
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
	
	//move the "camera" two units toward us, look at the origin, +y is up
	//lookAt() is equivalent to R_{WV} * E, the transpose of the camera-to-world orientation matrix times the camera's translation matrix
    //ViewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	//LightMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    
	GameObject* object = new GameObject("container.obj", 1);
	GameObject* object2 = new GameObject("container.obj", 2);
	gameObjects.push_back(object);
	gameObjects.push_back(object2);

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

void game_loop(void){
    int playing;
    playing = GL_TRUE;
    while( playing && glfwGetWindowParam( GLFW_OPENED ) )
    {        
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

void CreateCube()
{
   	printf("Create\n");

    //we create a shader program, attach to shaders to it (vertex and fragment), then link
    /*shaderProgramID = glCreateProgram();
    printf("%d\n",shaderProgramID);
    OnGLError("ERROR: Could not create the shader program");
	
	fragmentShaderID = LoadShader("container.fs", GL_FRAGMENT_SHADER);
	vertexShaderID = LoadShader("container.vs", GL_VERTEX_SHADER);
	
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	
	//if not using "location" in shader
	glBindAttribLocation(shaderProgramID, 0, "in_Position");
	glBindAttribLocation(shaderProgramID, 1, "in_Tex");
	glBindAttribLocation(shaderProgramID, 2, "in_Normal");

	glLinkProgram(shaderProgramID);
    OnGLError("ERROR: Could not link the shader program");

    //Uniform variables that will be updated every draw call
	ViewMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "ViewMatrix");
	ProjectionMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "ProjectionMatrix");
	LightMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "LightMatrix");
    OnGLError("ERROR: Could not get shader uniform locations");*/
    
    //Create the VAO, the object that specifies how the vertex data is organized
	/*glGenVertexArrays(1, &VaoID);
    OnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(VaoID);
    OnGLError("ERROR: Could not bind the VAO");

    //we have two vertex attributes that we care about, and our shader is set up to take in position as the first argument
    //and color as the second
	glEnableVertexAttribArray(0); //in_Position
	glEnableVertexAttribArray(1); //in_Color
    OnGLError("ERROR: Could not enable vertex attributes");*/

	OnGLError("ERROR 6");

	//Create all the things
	for(int i = 0; i < gameObjects.size(); i++)
	{
		//std::cout << i << std::endl;
		//OnGLError("SUPER BIG ERROR");
		gameObjects[i]->Create(shaderProgramID);
	}
}

void DrawCube(void)
{
	Now = glfwGetTime();

	if (LastTime == 0)
		LastTime = Now;

	LastTime = Now;
    
    //clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	//glUseProgram(shaderProgramID);
    //OnGLError("DRAW_ERROR: Could not use the shader program");

	//Update all the things
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->UpdateModelMatrix();
	}

    //update uniform variables for this frame
	/*glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	glUniformMatrix4fv(LightMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(LightMatrix));
    OnGLError("ERROR: Could not set the shader uniforms");*/
	
	//glBindVertexArray(VaoID);
    //OnGLError("ERROR: Could not bind the VAO for drawing purposes");
    
    //glDrawElements draws the indices of the active GL_ARRAY_BUFFER as specified by the buffer bound to the
    //GL_ELEMENT_ARRAY_BUFFER target (the IBO)
    
	//Draw all the things
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Draw();
	}

    //not necessary right now, but a good habit
	//glBindVertexArray(0);
	//glUseProgram(0);
}

/*void DestroyCube()
{
	glDetachShader(shaderProgramID, vertexShaderID);
	glDetachShader(shaderProgramID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(shaderProgramID);
	OnGLError("ERROR: Could not destroy the shaders");

	glDeleteBuffers(2, &VboID);
	glDeleteVertexArrays(1, &VaoID);
	OnGLError("ERROR: Could not destroy the buffer objects");
}*/


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
