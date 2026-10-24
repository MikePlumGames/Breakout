#version 330 core
layout (location = 0) in vec2 aPos; 


uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * modelMatrix * vec4(aPos.xy, 0.0, 1.0);
}