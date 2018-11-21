#version 150
in  vec3 vertexPosition;
in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 camPosition;
out vec4 out_Color;

const vec3 ambientColor = vec3(1.0f,1.0f,1.0f)*0.01;
const vec3 lightPosition = vec3(2.0f,2.0f,2.0f);
const float lightIntensity = 10.0f;
const vec3 lightColor = vec3(1.0f,1.0f,1.0f);
const float gloss = 15.0f;
const float screenGamma = 1.5;

void main() {
  //comment!
  vec3 normal = normalize(pass_Normal);
  vec3 lightDirection = lightPosition - vertexPosition;
  float distance = length(lightDirection) * length(lightDirection);
  lightDirection = normalize(lightDirection);
  float lambertiano = max(dot(lightDirection, normal), 0.0);
  float specular = 0.0;

  if(lambertiano > 0.0){
    vec3 viewDirection = normalize(-vertexPosition);
    vec3 halfDirection = normalize(lightDirection + viewDirection);
    float specularAngle = max(dot(halfDirection, normal), 0.0);
    specular = pow(specularAngle, gloss);
  }
  vec3 colorLinear = 
    ambientColor + 
    pass_Color*lambertiano*lightColor*lightIntensity/distance +
    pass_Color*specular*lightColor*lightIntensity/distance;
  
  //gamma adjusting
  colorLinear = pow(colorLinear,vec3(1.0/screenGamma));
            
  out_Color = vec4(colorLinear, 1.0);
}
