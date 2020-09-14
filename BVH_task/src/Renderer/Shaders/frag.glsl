#version 330 core
 
uniform vec4 vertexCol;
//uniform vec3 lightPos;

//in vec3 Normal;
//in vec3 FragPos;

out vec4 FragColor;

void main() 
{
    //vec3 normal = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    //float diffuse = max(dot(normal, lightDir), 0.5);

    //float ambientStrength = 0.1;
    //vec3 ambientLight = normalize(-1.0, -1.5, -0.8);
    //vec3 ambient = ambientStrength * ambientLight;
 
    // FragColor = vec4(0.4f, 0.2f, 0.8f, 1.0f);
    FragColor = vertexCol;
}
