#version 150

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 LightMatrix;

out vec3 LCpos;            // Vertex position in light coordinates
out vec3 LCnorm;           // Normal in light coordinates
out vec3 LCcamera;         // Camera position in light coordinates

in vec4 in_Position;
in vec2 in_Tex;
in vec3 in_Normal;

out vec2 v_tex;

void main()
{
    gl_Position = (ProjectionMatrix * ViewMatrix * ModelMatrix) * in_Position;

    vec4 ViewPosition = ViewMatrix[3];
    mat4 WCtoLC = transpose(inverse(LightMatrix));
    mat4 WCtoLCit = inverse(WCtoLC);
    mat4 MCtoWC = ModelMatrix;
    mat4 MCtoWCit = MCtoWC;

    // Compute world space position and normal
    vec4 wcPos = ModelMatrix * in_Position;
    vec3 wcNorm = (MCtoWCit * vec4(in_Normal, 0.0)).xyz;

    // Compute light coordinate system camera position,
    // vertex position and normal
    LCcamera = (WCtoLC * ViewPosition).xyz;
    LCpos = (WCtoLC * wcPos).xyz;
    LCnorm = (WCtoLCit * vec4(wcNorm, 0.0)).xyz;
    
    v_tex = in_Tex;
    
}