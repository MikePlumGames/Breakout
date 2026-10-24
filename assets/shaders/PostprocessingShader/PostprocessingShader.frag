#version 330 core

in  vec2  TexCoords;

out vec4  color;
  
uniform sampler2D image;            // image of the scene
uniform vec2      offsets[9];
uniform int       edge_kernel[9];
uniform float     blur_kernel[9];

// all the postprocessing efects
uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main()
{
    color = vec4(0.0f);

    vec3 imageSample[9];

    // sample from texture offsets if using convolution matrix
    if(chaos || shake)
    {
        for(int i = 0; i < 9; i++)
            imageSample[i] = vec3(texture(image, TexCoords.st + offsets[i]));
    }

    // chaos postprocessing efect
    if (chaos)
    {           
        for(int i = 0; i < 9; i++)
            color += vec4(imageSample[i] * edge_kernel[i], 0.0f);
        color.a = 1.0f;
    }
    // confuse postprocessing efect
    else if (confuse)
    {
        color = vec4(1.0 - texture(image, TexCoords).rgb, 1.0); // reverse the color
    }
    // shake postprocessing efect
    else if (shake)
    {
        for(int i = 0; i < 9; i++)
            color += vec4(imageSample[i] * blur_kernel[i], 0.0f);
        color.a = 1.0f;
    }
    // no effects
    else
    {
        color =  texture(image, TexCoords);
    }
}