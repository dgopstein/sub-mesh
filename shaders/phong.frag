#version 330

// http://stackoverflow.com/questions/4703432/why-does-my-opengl-phong-shader-behave-like-a-flat-shader

out vec4 vFragColor;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
smooth in vec3 vVaryingNormal;
smooth in vec3 vVaryingLightDir;

void main(void) {
 float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));
 vFragColor = diff * diffuseColor;
 vFragColor += ambientColor;
 vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),normalize(vVaryingNormal)));
 float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));

 float fSpec = pow(spec, 32.0);
 vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
}
