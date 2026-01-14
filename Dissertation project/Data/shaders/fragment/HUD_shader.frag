#version 330 core
out vec4 FragColor;

in vec3 Colour;

in vec3 Normal;  
in vec3 FragPos;

void main()
{        
    vec3 result = Colour;

    FragColor = vec4(result, 1.0);
} 