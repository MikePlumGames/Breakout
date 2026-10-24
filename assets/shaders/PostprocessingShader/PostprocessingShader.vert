#version 330 core
layout (location = 0) in vec4 aVertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

// all the postprocessing efects
uniform bool  chaos;
uniform bool  confuse;
uniform bool  shake;

uniform float time;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * modelMatrix * vec4(aVertex.xy, 0.0f, 1.0f); 
    vec2 textureCoords = aVertex.zw;

    // chaos postprocessing efect
    if (chaos)
    {
        float strength = 0.3;
        vec2 pos = vec2(textureCoords.x + sin(time) * strength, textureCoords.y + cos(time) * strength);        
        TexCoords = pos;
    }

    // confuse postprocessing efect
    else if (confuse)
    {
        TexCoords = vec2(1.0 - textureCoords.x, 1.0 - textureCoords.y);
    }

    // neither chaos nor confuse
    else
    {
        TexCoords = textureCoords;
    }

    // shake postprocessing efect
    if (shake)
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;        
        gl_Position.y += cos(time * 15) * strength;        
    }
}  