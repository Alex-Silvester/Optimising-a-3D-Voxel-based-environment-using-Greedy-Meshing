#version 330 core
out vec4 FragColor;

in vec3 Colour;

in vec3 Normal;  
in vec3 FragPos;  
in vec3 GlobalPos;

uniform vec3 worldSize;
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float intensity;
uniform float ambientStrength;
uniform vec3 scale;

float near = 0.1; 
float far  = 100.0; 

float DepthVisualiser(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near))/far;	
}

float notOnEdge()
{
    // Compute anti-aliased world-space grid lines
    vec3 grid = abs(fract(FragPos) - 0.5) / fwidth(FragPos);
    float line = min(min(grid.x, grid.y), grid.z);

    // Just visualize the grid lines directly
    float color = min(line, 1.0);

    // Apply gamma correction
    color = pow(color, 1.0 / 2.2);

    return color; 
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

    vec3 normalPos = vec3(GlobalPos.x/worldSize.x,GlobalPos.y/worldSize.y,GlobalPos.z/worldSize.z);
    FragColor = vec4(result * notOnEdge(), 1.0);
} 