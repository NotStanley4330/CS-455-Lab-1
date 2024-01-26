#version 330 core
out vec4 ColorOut;

uniform vec3 ColorOutValue;  // Use the same name as in your C++ code

void main()
{
    ColorOut = vec4(ColorOutValue, 1.0f);
}