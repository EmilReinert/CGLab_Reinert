#version 150

in  vec3 pass_Normal;
in  vec3 pass_Color;
out vec4 out_Color;
vec3 lightPosition = vec3(0,0,0);

void main() {
  out_Color = vec4(pass_Color,1.0);
}
