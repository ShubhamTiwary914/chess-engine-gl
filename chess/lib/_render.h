#ifndef RENDER_H
#define RENDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
#include "utils/gameutils.h"


class GameObject {
public:
    glm::vec2 position;
    glm::vec2 scale;
    glm::vec3 color;
    unsigned int VAO;
    Shader shader;
    float aspect;

    GameObject(unsigned int vao, Shader& pieceShader) : VAO(vao), shader(pieceShader) {
        aspect = getAspectRatio();
    }
};


class RenderBoard : public GameObject {
public:
    RenderBoard(unsigned int vao, Shader& boardShader) : GameObject(vao, boardShader) {}
    void render() {
        shader.use();
        shader.setFloat("aspectRatio", aspect);
        
        // Draw Grid(Instances)
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


class RenderPieces : public GameObject {
public:
    RenderPieces(unsigned int vao, Shader& pieceShader) : GameObject(vao, pieceShader) {}
    
    void render(char pieceKey, char file, int rank, bool flipPiece = true) {
        shader.use();
        shader.setFloat("aspectRatio", aspect);
        
        // Calculate grid position from chess coordinates
        int fileIndex = parseFileID(file);  // A-H -> 0-7
        int rankIndex = rank - 1;           // 1-8 -> 0-7
        
        // Set piece position
        glm::vec2 position(fileIndex, rankIndex);
        shader.setVec2("pieceOffset", position);
        
        // Set flip state for shader
        shader.setBool("flipTexture", flipPiece);
        
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pieceTextures[pieceKey].id);
        shader.setInt("texture1", 0);
        
        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Draw piece
        glBindVertexArray(VAO);
        glDrawElements(
            GL_TRIANGLES,
            6,              // 6 indices for a quad
            GL_UNSIGNED_INT,
            0
        );
        
        glDisable(GL_BLEND);
        glBindVertexArray(0);
    }

    void updateAspectRatio() {
        aspect = getAspectRatio();
        shader.use();
        shader.setFloat("aspectRatio", aspect);
    }
};

#endif