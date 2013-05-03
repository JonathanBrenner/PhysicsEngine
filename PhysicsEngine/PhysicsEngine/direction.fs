#version 150

in vec2 v_tex;
in float nDotVP;
in float pf;
uniform sampler2D s_tex;

out vec4 FragColor;

const vec3 ambient = vec3(0.5, 0.5, 0.5);
const vec3 diffuse = vec3(0.6, 0.9, 0.6);
const vec3 specular = vec3(0.7, 0.7, 0.7);

void main(void)
{
	FragColor = texture(s_tex, v_tex) * vec4((ambient + diffuse*nDotVP + specular*pf), 1);
}