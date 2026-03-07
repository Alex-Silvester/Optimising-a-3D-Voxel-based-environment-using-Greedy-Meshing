#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec3 aNormal;

out vec3 Colour;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 position;

uniform vec3 scale;

void main()
{
	mat4 model_view = view * model;
	
	model_view[0][0] = 2 * scale.x;
	model_view[0][1] = 0;
	model_view[0][2] = 0;
	
	model_view[1][0] = 0;
	model_view[1][1] = 2 * scale.y; 
	model_view[1][2] = 0;
	
	model_view[2][0] = 0;
	model_view[2][1] = 0;
	model_view[2][2] = 2 * scale.z;
	
	gl_Position = projection * (model_view * vec4(aPos, 1.0) + model * view * vec4(2*position,1.0));
	
	Colour = aColour;
}