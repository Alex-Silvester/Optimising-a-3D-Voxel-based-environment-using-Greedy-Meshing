#version 330 core
out vec4 FragColor;

in vec3 Colour;
in vec3 Normal;  
in vec3 FragPos;

uniform float alpha;

void main()
{        
    FragColor = vec4(Colour,alpha);
} 