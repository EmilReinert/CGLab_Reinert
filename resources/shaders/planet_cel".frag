#version 150

in  vec3 vertexPosition;
in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 camPosition;
out vec4 out_Color;

const vec3 ambientColor = vec3(1.0f,1.0f,1.0f)*0.005;
const vec3 lightPosition = vec3(2.0f,2.0f,2.0f);
const float lightIntensity = 2.0f;
const vec3 lightColor = vec3(1.0f,1.0f,1.0f);
const float gloss = 15.0f;
const float screenGamma = 1.5;

void main() {  //comment!
  vec3 normal = normalize(pass_Normal+4);
  vec3 lightDirection = vec3(0.0,0.0,100)+camPosition;
  float distance = 50;
  lightDirection = normalize(lightDirection);
  float lambertiano = max(dot(lightDirection, normal), 0.0);


  vec3 colorLinear = 
    ambientColor + 
    pass_Color*lambertiano*lightColor*lightIntensity/distance;
  
            
  out_Color = vec4(colorLinear, 1.0);
}
