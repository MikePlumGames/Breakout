#include "Renderer.h"

#include "Breakout.h"
#include "ShaderLoader.h"

#include <glad/glad.h>		// openGL functions
#include <glm/glm.hpp>		// matricies and vectores
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <string>

// render a quad without texture
void renderQuad(glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, glm::vec2 target)
{
    // static makes the shader only compile once
    static Shader shader("assets/shaders/QuadShader/QuadShader.vert",
        "assets/shaders/QuadShader/QuadShader.frag");

    static bool isVAOConfigured{ false };
    static unsigned int VAO{};

    // this ensures VAO is bound once
    if (!isVAOConfigured)
    {
        // create the VAO
        float vertices[] = {
            // pos
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

        unsigned int VBO;
        // create 
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // configure VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // configure VAO
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        // unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // so it's only updated once
        isVAOConfigured = true;
    }


    // render the quad

    // transform the model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // translate
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));
    // rotate - we first translate to make the origin 0,0 in the center instead of top left so it rotates around the center
    // multiplication by size bc we already scaled the matrix (matricies are multiplied in reverse) to get to the actual center of the object
    // this tranformation is undone afterwards
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    // scale
    modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));

    glm::mat4 projectionMatrix = glm::ortho(0.0f, target.x, target.y, 0.0f, g_closePlane, g_farPlane);

    // set the uniforms
    shader.use();
    shader.setMat4("modelMatrix", modelMatrix);
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setVec4("spriteColor", color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// render a quad with texture
void renderQuadTexture(unsigned int texture, glm::vec2 position, glm::vec2 size, float rotation, glm::vec4 color, glm::vec2 target)
{
    // static makes the shader only compile once
    static Shader shader("assets/shaders/QuadTextureShader/QuadTextureShader.vert",
                         "assets/shaders/QuadTextureShader/QuadTextureShader.frag");

    static bool isVAOConfigured{ false };
    static unsigned int VAO{};

    // this ensures VAO is bound once
    if (!isVAOConfigured)
    {
        // create the VAO
        float vertices[] = {
            // pos      // TexCoords
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

        unsigned int VBO;
        // create 
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // configure VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // configure VAO
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        // unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // so it's only updated once
        isVAOConfigured = true;
    }

	
    // render the quad

    // transform the model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // translate
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));
    // rotate - we first translate to make the origin 0,0 in the center instead of top left so it rotates around the center
    // multiplication by size bc we already scaled the matrix (matricies are multiplied in reverse) to get to the actual center of the object
    // this tranformation is undone afterwards
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    // scale
    modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));
    
    glm::mat4 projectionMatrix = glm::ortho(0.0f, target.x, target.y, 0.0f, g_closePlane, g_farPlane);

    // set the uniforms
    shader.use();
    shader.setMat4("modelMatrix", modelMatrix);
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setVec4("spriteColor", color);
    shader.setInt("image", 0);

    // assigne the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void renderQuadSimple(glm::vec2 position, float scale, glm::vec4 color, glm::vec2 target)
{
    // static makes the shader only compile once
    static Shader shader("assets/shaders/QuadShader/QuadShader.vert",
        "assets/shaders/QuadShader/QuadShader.frag");

    static bool isVAOConfigured{ false };
    static unsigned int VAO{};

    // this ensures VAO is bound once
    if (!isVAOConfigured)
    {
        // create the VAO
        float vertices[] = {
            // pos
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

        unsigned int VBO;
        // create 
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // configure VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // configure VAO
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        // unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // so it's only updated once
        isVAOConfigured = true;
    }
    // render the quad

    glm::vec2 size = glm::vec2(scale);

    // transform the model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // translate
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));
  
    // scale
    modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));

    glm::mat4 projectionMatrix = glm::ortho(0.0f, target.x, target.y, 0.0f, g_closePlane, g_farPlane);

    // set the uniforms
    shader.use();
    shader.setMat4("modelMatrix", modelMatrix);
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setVec4("spriteColor", color);


    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// render a quad with texture
void renderPostprocessedQuad(unsigned int texture, glm::vec2 position, glm::vec2 size, float rotation, bool shake, bool confuse, bool chaos, glm::vec2 target)
{
    // static makes the shader only compile once
    static Shader shader("assets/shaders/PostprocessingShader/PostprocessingShader.vert",
        "assets/shaders/PostprocessingShader/PostprocessingShader.frag");

    static bool isVAOandShaderConfigured{ false };
    static unsigned int VAO{};

    // this ensures VAO is bound once, we also send some data to the shader here
    if (!isVAOandShaderConfigured)
    {
        // create the VAO
        float vertices[] = {
            // pos      // TexCoords    y flipped: 0.0 -> 1.0 & 1.0 -> 0.0 bc fbo's use different coord system (0.0 at the bottom left) and we use 0.0 at the top left
            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 1.0f
        };

        unsigned int VBO;
        // create 
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // configure VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // configure VAO
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        // unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // shader data
        shader.use();

        float offset = 1.0f / 300.0f;
        glm::vec2 offsets[9] = {
            { -offset,  offset  },  // top-left
            {  0.0f,    offset  },  // top-center
            {  offset,  offset  },  // top-right
            { -offset,  0.0f    },  // center-left
            {  0.0f,    0.0f    },  // center-center
            {  offset,  0.0f    },  // center - right
            { -offset, -offset  },  // bottom-left
            {  0.0f,   -offset  },  // bottom-center
            {  offset, -offset  }   // bottom-right    
        };

        int edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
        };

        float blur_kernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
        };

        for (int i = 0; i < 9; i++)
        {
            // offsets
            std::string location{ "offsets[" + std::to_string(i) + "]" };
            shader.setVec2(location, offsets[i]);
            // edge_kernel
            location =  "edge_kernel[" + std::to_string(i) + "]";
            shader.setInt(location, edge_kernel[i]);
            // blur_kernel
            location = "blur_kernel[" + std::to_string(i) + "]";
            shader.setFloat(location, blur_kernel[i]);
        }
        //glUniform2fv(glGetUniformLocation(shader.ID, "offsets"), 9, (float*)offsets);   // set the offsets array

        // so it's only updated once
        isVAOandShaderConfigured = true;
    }


    // render the quad

    // transform the model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    // translate
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));
    // rotate - we first translate to make the origin 0,0 in the center instead of top left so it rotates around the center
    // multiplication by size bc we already scaled the matrix (matricies are multiplied in reverse) to get to the actual center of the object
    // this tranformation is undone afterwards
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
    // scale
    modelMatrix = glm::scale(modelMatrix, glm::vec3(size, 1.0f));

    // for the final quad we create a different projection using g_screenWidth instead of g_gameWidth so we render the final quad in the full window
    glm::mat4 projectionMatrix = glm::ortho(0.0f, target.x, target.y, 0.0f, g_closePlane, g_farPlane);

    float time{ getTime() };

    // set the uniforms
    shader.use();
    // vertex shader
    shader.setMat4("modelMatrix", modelMatrix);
    shader.setMat4("projectionMatrix", projectionMatrix);
    shader.setFloat("time", time);
    // both shaders
    shader.setBool("chaos", chaos);
    shader.setBool("confuse", confuse);
    shader.setBool("shake", shake);
    //fragment shader
    shader.setInt("image", 0);
    // the rest is passed above in the if statement as we only need to pass it once

    // assigne the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}