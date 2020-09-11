#version 330 core
out vec4 FragColor;
 
uniform vec4 vertexCol;
void main() 
{
    // FragColor = vec4(0.4f, 0.2f, 0.8f, 1.0f);
    FragColor = vertexCol;
}
