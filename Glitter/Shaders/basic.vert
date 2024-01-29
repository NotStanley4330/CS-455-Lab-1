#version 330 core
in vec3 basicPosition;
uniform mat4 basicTransform;
in vec3 basicColor;
out vec3 color;

void main()
{
    gl_Position = basicTransform * vec4(basicPosition, 1.0) ;
    color = basicColor;
}