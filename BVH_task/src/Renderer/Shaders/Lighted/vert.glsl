#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;


uniform mat4 proj; 
uniform mat4 view; 
uniform mat4 model; 

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);;
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = normal;
}