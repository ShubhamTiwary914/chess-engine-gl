#ifndef GLCONTROL_H
#define GLCONTROL_H
//Gl frameworks
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "lib/stb_image.h"
//standard
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>

typedef struct {
    unsigned int id;
    int width;
    int height;
} PieceTexture;

std::map<char, PieceTexture> pieceTextures;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void setVertexAttributes(unsigned int &VBO, unsigned int &instanceVBO, unsigned int &EBO, unsigned int &VAO, 
                    std::vector<float> &cellVertices, std::vector<unsigned int> &indices, std::vector<glm::vec2> &gridPositions){

    // Create and bind VAO 
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Setup vertex buffer (single square)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cellVertices.size() * sizeof(float), cellVertices.data(), GL_STATIC_DRAW);

    // Position attributes (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coordinates (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Setup instance buffer (grid positions)
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, gridPositions.size() * sizeof(glm::vec2), gridPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // Instanced vertex attribute
    
    // Setup element buffer (indices for single cell)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Unbind VAO to prevent accidental modifications
    glBindVertexArray(0);
};

GLFWwindow *init(const char* TITLE){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create & check window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    //check display size
    int displayW, displayH;
    glfwGetFramebufferSize(window, &displayW, &displayH);
    float aspect = (float)displayW / (float)displayH;
    std::cout << "Window aspect ratio: " << aspect << std::endl;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //check GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//key in range(ASCII (A-65) to (Z-97))
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    //escape -> exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//> Textures Handlers
std::string getAssetsPath() {
    // Get the current executable's path
    std::filesystem::path execPath = std::filesystem::current_path();
    // Construct path to assets
    std::filesystem::path assetsPath = execPath / "assets";
    return assetsPath.string();
}

void loadTexture(const char* path, unsigned int &textureID) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
}

void loadPieceTextures() {
    // Piece characters (uppercase for white, lowercase for black)
    const char* pieces = "KQRBNPkqrbnp";  // White pieces then black pieces
    std::string basePath = getAssetsPath();
    
    for(const char* p = pieces; *p; ++p) {
        char key = *p;  // Use char directly
        std::string fullPath = basePath + "/" + std::string(1, key) + ".png";
        
        PieceTexture texture;
        loadTexture(fullPath.c_str(), texture.id);
        pieceTextures[key] = texture;
    }
}

#endif