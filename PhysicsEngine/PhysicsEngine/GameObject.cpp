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
	transform = Transform();
}

GameObject::GameObject(std::string pathName)
{
	//GameObject();

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

void GameObject::Create(GLuint shaderProgramID)
{
	modelMatrixUniformLocation = glGetUniformLocation(shaderProgramID, "ModelMatrix");

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

	glBindVertexArray(0);

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
    
    GLuint samplerLoc = glGetUniformLocation(shaderProgramID, "s_tex");

    glUniform1i(samplerLoc, 0);
}

void GameObject::UpdateModelMatrix()
{
	glUniformMatrix4fv(modelMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(transform.modelMatrix));
}

void GameObject::Draw()
{
	glDrawElements(GL_TRIANGLES, sizeof(indices[0]) * indices.size()/sizeof(GLuint), GL_UNSIGNED_INT, (GLvoid*)0);
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