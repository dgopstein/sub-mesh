#version 330

// http://stackoverflow.com/questions/4703432/why-does-my-opengl-phong-shader-behave-like-a-flat-shader

in vec4 vVertex;
in vec3 vNormal;
uniform mat4 mvpMatrix;  // mvp = ModelViewProjection
uniform mat4 mvMatrix; // mv = ModelView
uniform mat3 normalMatrix;
uniform vec3 vLightPosition;
smooth out vec3 vVaryingNormal;
smooth out vec3 vVaryingLightDir;

void main(void) {
 vVaryingNormal = normalMatrix * vNormal;
 vec4 vPosition4 = mvMatrix * vVertex;
 vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
 vVaryingLightDir = normalize(vLightPosition - vPosition3);
 gl_Position = mvpMatrix * vVertex;
}
