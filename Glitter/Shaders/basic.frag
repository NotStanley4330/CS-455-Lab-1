#version 330 core
in vec3 color;
out vec4 ColorOut;
uniform vec3 colorTransform;

void main()
{
    ColorOut = vec4(colorTransform * color, 1.0f);
}