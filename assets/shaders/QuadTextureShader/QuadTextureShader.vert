#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{
    TexCoords = aVertex.zw;
    gl_Position = projectionMatrix * modelMatrix * vec4(aVertex.xy, 0.0, 1.0);
}