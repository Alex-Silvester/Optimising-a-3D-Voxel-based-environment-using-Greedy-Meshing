#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Colour;
in vec3 Normal;  
  
uniform vec3 lightPos; 
uniform float intensity;
uniform float ambientStrength;
uniform vec3 scale;

void main()
{ 	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    float diffuse = diff * intensity;
            
    vec3 result = (ambientStrength + diffuse) * Colour;

    FragColor = vec4(result, 1.0);
} 