#version 330

// http://stackoverflow.com/questions/4703432/why-does-my-opengl-phong-shader-behave-like-a-flat-shader

layout(location = 0) in vec3 vVertex3;
layout(location = 2) in vec3 vNormal;
uniform mat4 MVP;  // mvp = ModelViewProjection
uniform mat4 MV; // mv = ModelView
uniform mat4 normalMatrix;
uniform vec3 vLightPosition;
smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;

void main(void) {
 vec4 vVertex4 = vec4(vVertex3, 1);
 vVaryingNormal = vec3(normalMatrix * vec4(vNormal, 0));
 vec4 vPosition4 = MV * vVertex4;
 vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
 vVaryingLightDir = normalize(vLightPosition - vPosition3);
 gl_Position = MVP * vVertex4;
}
