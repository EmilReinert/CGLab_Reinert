#version 150
in  vec3 vertexPosition;
in  vec3 pass_Normal;
in  vec3 pass_Color;
in  vec3 camPosition;
in  vec2 pass_TexCoord;


out vec4 out_Color;

uniform sampler2D PlanetTexture;

vec3 ambientColor;
const vec3 lightPosition = vec3(0.0f,0.0f,0.0f);
const float lightIntensity = 1;
const vec3 lightColor = vec3(1.0f,1.0f,1.0f);
const float gloss = 4.0f;
const float screenGamma = 1.5;

void main() {
  // TODO setting new color based on texture
  vec3 color = vec3(texture(PlanetTexture, pass_TexCoord));
  ambientColor = color * 0.05;
  //comment!
  vec3 normal = normalize(pass_Normal);
  vec3 lightDirection = lightPosition - vertexPosition;
  float distance = length(lightDirection);
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
    color*lambertiano*lightColor*lightIntensity/distance +
    vec3(1.0, 1.0, 1.0)*specular*lightColor*lightIntensity/distance;
  
  //gamma adjusting
  colorLinear = pow(colorLinear,vec3(1.0/screenGamma));
            
            
  out_Color = vec4(colorLinear, 1.0);
  //out_Color = texture(PlanetTexture, pass_TexCoord);
}
