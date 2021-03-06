#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_Texcoord;

//Matrix Uniforms as specified with glUniformMatrix4fv

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform vec3 PlanetColor;


out vec3 vertexPosition;
out vec3 pass_Normal;
out vec3 pass_Color;
out vec3 camPosition;

out vec2 pass_TexCoord;

void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	
	vec4 vertPos4 = ModelMatrix * vec4(in_Position, 1.0);
	vertexPosition = vec3(vertPos4) / vertPos4.w;
	//camPosition = vec3(Viewatrix * ModelMatrix * NormalMatrix * vec4(in_Position, 0.0)).xyz;
	camPosition = (transpose(ViewMatrix)[3] * -1).xyz;
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	pass_Color = PlanetColor; 

	pass_TexCoord = in_Texcoord;
}
