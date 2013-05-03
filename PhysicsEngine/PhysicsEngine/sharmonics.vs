#version 150

in vec4 in_Position;
in vec2 in_Tex;
in vec3 in_Normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

const float C1 = 0.429043;
const float C2 = 0.511664;
const float C3 = 0.743125;
const float C4 = 0.886227;
const float C5 = 0.247708;

//Old Town Square Lighting
// const vec3 L00 = vec3(0.871297, 0.875222, 0.864470);
// const vec3 L1m1 = vec3(0.175058, 0.245335, 0.312891);
// const vec3 L10 = vec3(0.034675, 0.036107, 0.037362);
// const vec3 L11 = vec3(-0.004629, -0.029448, -0.048028);
// const vec3 L2m2 = vec3(-0.120535, -0.121160, -0.117507);
// const vec3 L2m1 = vec3(0.003242, 0.003624, 0.007511);
// const vec3 L20 = vec3(-0.028667, -0.024926, -0.020998);
// const vec3 L21 = vec3(-0.077539, -0.086325, -0.091591);
// const vec3 L22 = vec3(-0.161784, -0.191783, -0.219152);

//Eucalyptus Grove
const vec3 L00 = vec3(0.38, 0.43, 0.45);
const vec3 L1m1 = vec3(0.29, 0.36, 0.41);
const vec3 L10 = vec3(0.04, 0.03, 0.01);
const vec3 L11 = vec3(-0.1, -0.1, -0.09);
const vec3 L2m2 = vec3(-0.06, -0.06, -0.04);
const vec3 L2m1 = vec3(0.01, -0.01, -0.05);
const vec3 L20 = vec3(-0.09, -0.13, -0.15);
const vec3 L21 = vec3(-0.06, -0.05, -0.04);
const vec3 L22 = vec3(0.02, .00, -0.05);

//Galileo's tomb
// const vec3 L00 = vec3(1.04, 0.76, 0.71);
// const vec3 L1m1 = vec3(0.44, 0.34, 0.34);
// const vec3 L10 = vec3(-0.22, -0.18, -0.17);
// const vec3 L11 = vec3(0.71, 0.54, 0.56);
// const vec3 L2m2 = vec3(0.64, 0.5, 0.52);
// const vec3 L2m1 = vec3(-0.12, -0.09, -0.08);
// const vec3 L20 = vec3(-0.37, -0.28, -0.29);
// const vec3 L21 = vec3(-0.17, -0.13, -0.13);
// const vec3 L22 = vec3(0.55, 0.42, 0.42);


out vec3 DiffuseColor;
out vec2 v_tex;

void main(void)
{
    vec3 tnorm = normalize(NormalMatrix * in_Normal);
    
    DiffuseColor =  C1 * L22 * (tnorm.x * tnorm.x - tnorm.y * tnorm.y) +
                    C3 * L20 * tnorm.z * tnorm.z +
                    C4 * L00 -
                    C5 * L20 +
                    2.0 * C1 * L2m2 * tnorm.x * tnorm.y +
                    2.0 * C1 * L21 * tnorm.x * tnorm.z +
                    2.0 * C1 * L2m1 * tnorm.y + tnorm.z +
                    2.0 * C2 * L11 * tnorm.x +
                    2.0 * C2 * L1m1 * tnorm.y +
                    2.0 * C2 * L10 * tnorm.z;
                    
    v_tex = in_Tex;
	gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_Position;
}