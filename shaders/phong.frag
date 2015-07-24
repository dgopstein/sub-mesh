#version 330

// http://stackoverflow.com/questions/4703432/why-does-my-opengl-phong-shader-behave-like-a-flat-shader

out vec4 vFragColor;
//uniform vec4 ambientColor;
//uniform vec4 diffuseColor;
//uniform vec4 specularColor;
smooth in vec3 vVaryingNormal;
smooth in vec3 vVaryingLightDir;

const vec4 ambientColor  = 2.0 * vec4(.3, .2, .1, 0);
const vec4 diffuseColor  = 0.3 * vec4(.5, .9, .7, 0);
const vec4 specularColor = 0.02 * vec4(.6, .3, .9, 0);

void main(void) {
 float diff = max(0.0, dot(normalize(vVaryingNormal), normalize(vVaryingLightDir)));
 vFragColor = diff * diffuseColor;
 vFragColor += ambientColor;
 vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),normalize(vVaryingNormal)));
 float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));

 float fSpec = pow(spec, 128.0);
 vFragColor.rgb += vec3(fSpec, fSpec, fSpec);
}
