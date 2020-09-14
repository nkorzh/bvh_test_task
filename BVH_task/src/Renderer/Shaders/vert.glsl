#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;

uniform mat4 proj; 
uniform mat4 view; 
uniform mat4 model; 

//out vec3 Normal;
//out vec3 FragPos; 

void main()
{
    vec4 FragPos4 = model * vec4(position, 1.0);
    gl_Position = proj * view * FragPos4;
    //FragPos = vec3(FragPos4);
    //Normal = normal;
}