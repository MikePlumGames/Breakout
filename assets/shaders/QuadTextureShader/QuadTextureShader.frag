#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D image;
uniform vec4 spriteColor;

void main()
{    
    // so transparency works
    if (texture(image, TexCoords).a == 0.0)
        discard;

    color = vec4(spriteColor) * texture(image, TexCoords);
}  