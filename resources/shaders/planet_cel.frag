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
  vec3 normal = normalize(pass_Normal);
  vec3 reflect_dir = normalize(reflect(camPosition, normal));
  float cam_to_mirror_distance =length(reflect_dir-camPosition);
  float intensity = 0.1f;
  if(cam_to_mirror_distance<1.9999f){
    intensity = 1.0f;
  }  
  else{
    float intensity = 0.1f;
  }
  out_Color = vec4(normalize(pass_Color)*intensity, 1.0);
}
