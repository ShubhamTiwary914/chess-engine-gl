#define STB_IMAGE_IMPLEMENTATION
//Gl frameworks
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//standard
#include <iostream>
#include <vector>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);



void setVertexAttributes(unsigned int &VBO, unsigned int &instanceVBO, unsigned int &EBO, unsigned int &VAO, 
            std::vector<float> &cellVertices, std::vector<unsigned int> &indices, std::vector<glm::vec2> &gridPositions){
    // Create and bind VAO first
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Setup vertex buffer (single cell geometry)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cellVertices.size() * sizeof(float), cellVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Setup instance buffer (grid positions)
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, gridPositions.size() * sizeof(glm::vec2), gridPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1); // Tell OpenGL this is an instanced vertex attribute
    
    // Setup element buffer (indices for single cell)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Unbind VAO to prevent accidental modifications
    glBindVertexArray(0);
};


GLFWwindow *init(const int  WIDTH, const int  HEIGHT, const char* TITLE){
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