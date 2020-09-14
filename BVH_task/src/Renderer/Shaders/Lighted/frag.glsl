#version 330 core
 
uniform vec4 vertexCol;
uniform vec3 lightPos;
uniform bool countDiffuse;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main() 
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = lightColor;
    if (countDiffuse) {
        vec3 normal = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(normal, lightDir), 0.4);
        diffuse *= diff;
    }
          
    float ambientStrength = 0.0;
    vec3 ambientLight = normalize(vec3(-5.6, -4.5, -1.8));
    vec3 ambient = ambientStrength * ambientLight;
  
    vec3 objColor = vertexCol.xyz;
    FragColor = vec4((diffuse + ambient) * objColor, 0.8);
}