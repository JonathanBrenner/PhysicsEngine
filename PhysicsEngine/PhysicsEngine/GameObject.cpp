//
//  GameObject.cpp
//  PhysicsEngine
//
//  Created by Jonathan Brenner on 4/4/13.
//  Copyright (c) 2013 BrennerWeithers. All rights reserved.
//

#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::GameObject(std::string pathName, int index)
{
	viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	lightMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	projectionMatrix = glm::perspective(60.0f, 16.0f / 9.0f, 0.1f, 100.f); 

	shaderProgramID = index - 1;

	Vertex temp;
	temp.position[3] = 1;
	temp.uv[0] = 0;
	temp.uv[1] = 0;

	UV tempUV;
	Normal tempNormal;

	std::ifstream data("container.obj");
	std::string line;

	bool isVertex = false;
	bool isFace = false;
	bool isUV = false;
	bool isNormal = false;
	bool firstValue = true;
	int vertexOffset = 0;
	int faceOffset = 0;
	int uvOffset = 0;
	int normalOffset;
	float vertexIndex = 0;

	while(std::getline(data,line))
    {
		std::stringstream str(line);
		std::string text;

		isVertex = false;
		isFace = false;
		isUV = false;
		isNormal = false;
		firstValue = true;
		vertexOffset = 0;
		faceOffset = 0;
		uvOffset = 0;
		normalOffset = 0;
		vertexIndex = 0;
		
		while(std::getline(str, text, ' '))
		{
			std::stringstream str2(text);
			std::string text2;

			faceOffset = 0;

			while(std::getline(str2, text2, '/'))
			{
				if(isFace)
				{
					if(faceOffset == 0)
					{
						vertexIndex = ::atof(text2.c_str()) - 1;
						indices.push_back((GLuint)vertexIndex);
					}
					else if(faceOffset == 1)
					{
						if(vertices[vertexIndex].uv[0] == 0 && vertices[vertexIndex].uv[1] == 0)
						{
							float tempFloat = ::atof(text2.c_str()) - 1;
							vertices[vertexIndex].uv[0] = uvs[tempFloat].u;
							vertices[vertexIndex].uv[1] = uvs[tempFloat].v;
						}
					}
					else if(faceOffset == 2)
					{
						float tempFloat = ::atof(text2.c_str()) - 1;
						vertices[vertexIndex].normals[0] = normals[tempFloat].x;
						vertices[vertexIndex].normals[1] = normals[tempFloat].y;
						vertices[vertexIndex].normals[2] = normals[tempFloat].z;
					}

					faceOffset++;
				}
				else if(isVertex)
				{
					temp.position[vertexOffset] = ::atof(text2.c_str());

					if(vertexOffset == 2)
					{
						vertices.push_back(temp);
					}
					vertexOffset++;
					break;
				}
				else if(isUV)
				{
					if(uvOffset == 0)
					{
						tempUV.u = ::atof(text2.c_str());
					}
					else if(uvOffset == 1)
					{
						tempUV.v = ::atof(text2.c_str());
						uvs.push_back(tempUV);
					}
					uvOffset++;
				}
				else if(isNormal)
				{
					if(normalOffset == 0)
					{
						tempNormal.x = ::atof(text2.c_str());
					}
					else if(normalOffset == 1)
					{
						tempNormal.y = ::atof(text2.c_str());
					}
					else if(normalOffset == 2)
					{
						tempNormal.z = ::atof(text2.c_str());
						normals.push_back(tempNormal);
					}
					normalOffset++;
				}

				if(text2 == "v" && firstValue)
				{
					isVertex = true;
				}
				else if(text2 == "f" && firstValue)
				{
					isFace = true;
				}
				else if(text2 == "vt" && firstValue)
				{
					isUV = true;
				}
				else if(text2 == "vn" && firstValue)
				{
					isNormal = true;
				}

				firstValue = false;
			}
		}
    }
}

GameObject::GameObject(const GameObject& orig)
{
    
}

GameObject::~GameObject()
{
	glDetachShader(shaderProgramID, vertexShaderID);
	glDetachShader(shaderProgramID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteProgram(shaderProgramID);
	OnGLError("ERROR: Could not destroy the shaders");

	glDeleteBuffers(2, &vboID);
	glDeleteVertexArrays(1, &vaoID);
	OnGLError("ERROR: Could not destroy the buffer objects");
}

GLuint GameObject::LoadShader(const char* filename, GLenum shader_type)
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

void GameObject::Create(GLuint shaderProgramID1)
{
	shaderProgramID = glCreateProgram();
    
    OnGLError("ERROR: Could not create the shader program");
	
	fragmentShaderID = LoadShader("container.fs", GL_FRAGMENT_SHADER);
	vertexShaderID = LoadShader("container.vs", GL_VERTEX_SHADER);
	
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	
	//if not using "location" in shader
	glBindAttribLocation(shaderProgramID, 0, "in_position");
	glBindAttribLocation(shaderProgramID, 1, "in_tex");
	glBindAttribLocation(shaderProgramID, 2, "in_normal");

	glLinkProgram(shaderProgramID);
    OnGLError("ERROR: Could not link the shader program");

    //Uniform variables that will be updated every draw call
	viewMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "ViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "ProjectionMatrix");
	lightMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "LightMatrix");
    OnGLError("ERROR: Could not get shader uniform locations");

	modelMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "ModelMatrix");

	glGenVertexArrays(1, &vaoID);
    OnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(vaoID);
    OnGLError("ERROR: Could not bind the VAO");

    //we have two vertex attributes that we care about, and our shader is set up to take in position as the first argument
    //and color as the second
	glEnableVertexAttribArray(0); //in_Position
	glEnableVertexAttribArray(1); //in_Color
    OnGLError("ERROR: Could not enable vertex attributes");

    //create the VBO, the object that refers to the vertex data itself
	glGenBuffers(1, &vboID);
    OnGLError("ERROR: Could not generate the buffer objects");

    //make the VBO active, and send over the data!
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the VBO to the VAO");

    //this program gets unhappy if these two lines appear immediately after the glEnableVertexAttribArray() calls. Not sure why
    
    //for each attribute, we say how many values there are and in what type, whether to normalize them (no, we want the raw data), 
    //the size in bytes of a vertex, and the offset from the beginning of the vertex for the attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)sizeof(vertices[0].position));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)sizeof(vertices[0].position));
    OnGLError("ERROR: Could not set VAO attributes");

    //create the IBO, which contains the indices that specify the triangles to draw later
    glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	//send the index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
    OnGLError("ERROR: Could not bind the IBO to the VAO");

	glBindVertexArray(vaoID);

	// Generate texture objects
    glGenTextures( 1, &texID );

    // Make texture object active
    glBindTexture( GL_TEXTURE_2D, texID );

    // Set texture parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Upload texture from file to texture memory, auto-uses glTexImage2D, needs TGA
    if( !glfwLoadTexture2D( "container_diffuse.tga", 0 ) )
        fprintf( stderr, "Failed to load texture" );
        
    // since we're using mipmapping for the minification filter, we should tell texture unit to generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    samplerLoc = glGetUniformLocation(shaderProgramID, "s_tex");

	glUniform1i(samplerLoc, 0);
}

void GameObject::UpdateModelMatrix()
{
	glUseProgram(shaderProgramID);
    OnGLError("DRAW_ERROR: Could not use the shader program");

	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(transform.modelMatrix));
	glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(lightMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));
    OnGLError("ERROR: Could not set the shader uniforms");
}

void GameObject::Draw()
{
	glBindVertexArray(vaoID);
    OnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawElements(GL_TRIANGLES, (GLsizei)(sizeof(indices[0]) * indices.size() / sizeof(GLuint)), GL_UNSIGNED_INT, (GLvoid*)0);

	glBindVertexArray(vaoID);
	glUseProgram(shaderProgramID);
}


void GameObject::OnGLError(const char* error_message)
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