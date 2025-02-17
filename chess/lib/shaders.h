#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


//> Render Board Shader
const char* boardVertexShader = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;
    layout (location = 2) in vec2 aOffset;
    
    uniform float aspectRatio;
    flat out vec2 gridPos;
    
    void main() {
        gridPos = aOffset;
        
        // Convert 8x8 grid coordinates to [-1,1] range
        float x = (aOffset.x + aPos.x) / 4.0 - 1.0;
        float y = (aOffset.y + aPos.y) / 4.0 - 1.0;
        
        y = y + 0.11;
        x = x / aspectRatio;
        gl_Position = vec4(x, y, 0.0, 1.0);
    }
)";

const char* boardFragmentShader = R"(
    #version 330 core
    out vec4 FragColor;
    flat in vec2 gridPos;
    
    void main() {
        // black/white cell checker
        int row = int(gridPos.y);
        int col = int(gridPos.x);
        bool isWhite = ((row + col) % 2) == 0;
        
        vec3 whiteColor = vec3(0.93, 0.93, 0.82);
        vec3 blackColor = vec3(0.45, 0.32, 0.22);
        FragColor = vec4(isWhite ? whiteColor : blackColor, 1.0);
    }
)";


//> Pieces Shader
const char* pieceVertexShader = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoords;
    
    out vec2 TexCoords;
    uniform float aspectRatio;
    uniform vec2 pieceOffset;
    uniform bool flipTexture;
    
    void main() {
        // Flip texture coordinates in Y if needed
        TexCoords = flipTexture ? vec2(aTexCoords.x, 1.0 - aTexCoords.y) : aTexCoords;
        
        // Convert grid coordinates to [-1,1] range
        float x = (pieceOffset.x + aPos.x) / 4.0 - 1.0;
        float y = (pieceOffset.y + aPos.y) / 4.0 - 1.0;
        
        y = y + 0.11;
        x = x / aspectRatio;
        gl_Position = vec4(x, y, 0.0, 1.0);
    }
)";

const char* pieceFragmentShader = R"(
    #version 330 core
    out vec4 FragColor;
    in vec2 TexCoords;
    
    uniform sampler2D texture1;
    
    void main() {
        vec4 texColor = texture(texture1, TexCoords);
        if(texColor.a < 0.1) // Discard transparent pixels
            discard;
        FragColor = texColor;
    }
)";




class Shader {
public:
    unsigned int programID;
    std::string vertexCode;
    std::string fragmentCode;

    Shader(std::string vertexCode, std::string fragmentCode) {
        try {

            // Convert to const char* for shader source
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            // Log the loaded code
            std::cout << "Vertex & Fragment Shaders loaded" << std::endl;
            // std::cout << vShaderCode << std::endl;
            // std::cout << fShaderCode << std::endl;

            // 2. Compile shaders
            unsigned int vertex, fragment;

            // Vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");

            // Fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            // Shader program
            programID = glCreateProgram();
            glAttachShader(programID, vertex);
            glAttachShader(programID, fragment);
            glLinkProgram(programID);
            checkCompileErrors(programID, "PROGRAM");

            // Delete the shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);

        } catch (std::ifstream::failure &e) {
            std::cout << "ERROR>> " << e.what() << std::endl;
        }
    }


    void use() 
    { 
        glUseProgram(programID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(programID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value); 
    }
    void setVector4(const GLchar *name, GLfloat v1, GLfloat v2, GLfloat v3, GLfloat v4){
        int vertexLocation = glGetUniformLocation(this->programID, name);
        glUniform4f(vertexLocation, v1, v2, v3, v4);
    }

    // Add this method for vec2 uniforms
    void setVec2(const std::string &name, glm::vec2 value) const
    { 
        glUniform2f(glGetUniformLocation(programID, name.c_str()), value.x, value.y);
    }
    
    // Alternative overload that takes individual components
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(programID, name.c_str()), x, y);
    }

    private:
        void checkCompileErrors(unsigned int shader, std::string type) {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
};


#endif