#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
public:
    unsigned int programID;
    std::string vertexCode;
    std::string fragmentCode;

    Shader(std::string vertexPath, std::string fragmentPath) {
        try {
            // Load GLSL as raw string for OpenGL's shaders
            vertexCode = loadFileAsRawString(vertexPath);
            fragmentCode = loadFileAsRawString(fragmentPath);

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


    private:
        // Modify the return type to return a const char* (raw string literal)
        std::string loadFileAsRawString(const std::string& filename) {
            // Open the file in binary mode
            std::ifstream file(filename, std::ios::binary);
            if (!file) {
                std::cerr << "Error opening file: " << filename << std::endl;
                return "";
            }

            // Read the entire file content into a stringstream
            std::stringstream buffer;
            buffer << file.rdbuf();

            // Get the contents of the file as a string
            std::string fileContents = buffer.str();

            // Make sure the string is null-terminated
            fileContents.push_back('\0'); // Ensure null termination

            file.close();

            return fileContents;
        }

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

