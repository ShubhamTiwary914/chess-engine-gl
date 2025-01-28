#ifndef RENDER_H
#define RENDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.h"
#include "shaders.h"



class RenderBoard {
public:
    unsigned int VAO;
    Shader shader;
    float aspect;

    RenderBoard(unsigned int vao, Shader& boardShader) : VAO(vao), shader(boardShader) {
        aspect = getAspectRatio();
    }

    void render() {
        shader.use();
        shader.setFloat("aspectRatio", aspect);
        
        // Bind VAO and draw instanced grid
        glBindVertexArray(VAO);
        glDrawElementsInstanced(
            GL_TRIANGLES, 
            6,              // 6 indices for a quad
            GL_UNSIGNED_INT, 
            0,              // offset in EBO
            64             // 8x8 = 64 instances
        );
        glBindVertexArray(0);
    }

    void updateAspectRatio() {
        aspect = getAspectRatio();
        shader.use();
        shader.setFloat("aspectRatio", aspect);
    }
};

#endif