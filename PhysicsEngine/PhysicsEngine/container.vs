#version 150

in vec4 in_position;
in vec2 in_tex;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 v_tex;

void main(void)
{
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_position;
	v_tex = in_tex;
}