#version 150

in vec4 in_position;
in vec2 in_tex;
in vec3 in_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

const vec3 LightDirection = vec3(-0.5, 0.5, 3.0); //light is *emitted* in this direction
const float shininess = 50; //can be as high as 128 - bigger number = more concentrated bright spot

out vec2 v_tex;
out float nDotVP;
out float pf; //power factor

void main(void)
{
    //eye coordinate position
    vec3 ecPos = (ViewMatrix * ModelMatrix * in_position).xyz;
    vec3 eye = -normalize(ecPos);
    //halfvector, i.e. a vector with a direction half-way between the eye vector and the light vector, used in Blinn-Phong
    vec3 HV = normalize(normalize(LightDirection) + eye);
    vec3 tnorm = normalize(NormalMatrix * in_normal);
    
    nDotVP = max(0.0, dot(tnorm, normalize(LightDirection)));
    float nDotHV = max(0.0, dot(tnorm, HV));
    
    if (nDotVP == 0.0)
        pf = 0.0;
    else
        pf = pow(nDotHV, shininess);
    
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_position;
	v_tex = in_tex;
}