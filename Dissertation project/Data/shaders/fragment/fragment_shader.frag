#version 330 core
out vec4 FragColor;

in vec3 Colour;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float intensity;
uniform float ambientStrength;

float near = 0.1; 
float far  = 100.0; 

float DepthVisualiser(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near))/far;	
}

void main()
{
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * intensity;
            
    vec3 result = (ambient + diffuse) * objectColor * Colour;

    FragColor = vec4(result, 1.0);
} 