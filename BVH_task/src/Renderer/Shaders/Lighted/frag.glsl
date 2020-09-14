#version 330 core
 
uniform vec4 vertexCol;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main() 
{
    vec3 normal = normalize(Normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightColor = vec3(0.9, 0.8, 1.0);
    float diff = max(dot(normal, lightDir), 0.3);
    vec3 diffuse = diff * lightColor;
    
    float ambientStrength = 0.1;
    vec3 ambientLight = normalize(vec3(-5.6, -4.5, -1.8));
    vec3 ambient = ambientStrength * ambientLight;
 
    vec3 objColor = vertexCol.xyz;
    FragColor = vec4((diffuse + ambient) * objColor, 0.8);
    //FragColor = vec4(0.4, 0.1, 0.6, 1.0);
}