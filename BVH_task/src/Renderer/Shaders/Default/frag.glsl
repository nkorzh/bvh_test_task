#version 330 core
 
uniform vec4 vertexCol;

out vec4 FragColor;

void main() 
{
    FragColor = vertexCol;
}