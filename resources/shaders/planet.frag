#version 150
in  vec3 vertexPosition;
in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 camPosition;
out vec4 out_Color;

vec3 ambientColor;
const vec3 lightPosition = vec3(0.0f,0.0f,0.0f);
const float lightIntensity = 0.50f;
const vec3 lightColor = vec3(1.0f,1.0f,1.0f);
const float gloss = 4.0f;
const float screenGamma = 1.5;

void main() {
  ambientColor = pass_Color * 0.01;
  //comment!
  vec3 normal = normalize(pass_Normal);
  vec3 lightDirection = lightPosition - vertexPosition;
  float distance = length(lightDirection) * length(lightDirection);
  lightDirection = normalize(lightDirection);
  float lambertiano = max(dot(lightDirection, normal), 0.0);
  float specular = 0.0;

  vec3 viewDirection = normalize(camPosition - vertexPosition);
  vec3 halfDirection = normalize(lightDirection + viewDirection);
  float specularAngle = max(dot(halfDirection, normal), 0.0);
  //specular only determines shape/size of reflection not intensity
  specular = pow(specularAngle, 4.0*gloss);
  vec3 colorLinear = 
    ambientColor + 
    pass_Color*lambertiano*lightColor*lightIntensity/distance +
    vec3(1.0, 1.0, 1.0)*specular*lightColor*lightIntensity/distance;
  
  //gamma adjusting
  colorLinear = pow(colorLinear,vec3(1.0/screenGamma));
            
            
  out_Color = vec4(colorLinear, 1.0);
}
