#define STB_IMAGE_IMPLEMENTATION
//Gl frameworks
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//external libs
#include "lib/stb_image.h"
#include "lib/shaders.h"
#include "lib/objects.h"
#include "lib/utils.h"
#include "consts.h"
//in built
#include <iostream>
#include<vector>
#include <unordered_map>

//States
std::vector<float> vertices = {
    //Player rect
   -0.02f, -0.25f,  // Bottom-left corner (0-1)
    0.02f, -0.25f,   // Bottom-right corner (2-3)
    0.02f,  0.25f,   // Top-right corner  (4-5)
   -0.02f,  0.25f   // Top-left corner (6-7)
};
std::vector<unsigned int> indices = { 
    0, 1, 2, 
    2, 3, 0
}; 
std::unordered_map<char, bool> keyStates;



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow *init(const int  WIDTH,const int  HEIGHT, const char* TITLE);
void setVertexOvjects(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void setVertexBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);



int gameLoop(const int WIDTH, const int HEIGHT, const char* TITLE, const float BG_COLOR[]){
    //Add vertices & indices for circle
    appendCircle(vertices, indices, center[0], center[1], radius, segments);
    
    //Initialise window, event-handler, controls
    GLFWwindow* window = init(WIDTH, HEIGHT, TITLE);
    if(window == nullptr) return -1;
    glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //objects: shader, vertex buffers, game objects
    Shader ourShader(vertexShader, fragmentShader);    
    GLuint VBO, VAO, EBO;
    setVertexBuffers(VAO, VBO, EBO);
    PlayerRect p1(pos1, size, color, p1Keys);
    PlayerRect p2(pos2, size, color, p2Keys);
    Ball ball(center, ballSize, ballColor, segments);


    //>event loop
	while(!glfwWindowShouldClose(window))
	{
        glClearColor(BG_COLOR[0]/255, BG_COLOR[1]/255, BG_COLOR[2]/255, 0.2f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Game Objects
        ourShader.use();
        glBindVertexArray(VAO);
        //Players
        p1.playerControls(deltaTime, keyStates); 
        p1.renderRectangle(ourShader.programID);
        p2.playerControls(deltaTime, keyStates); 
        p2.renderRectangle(ourShader.programID);
        //Ball
        ball.pongMovements(deltaTime, p1, p2); //+ hitbox, gameover-trigger
        ball.renderBall(ourShader.programID);
	    glfwSwapBuffers(window);
	    glfwPollEvents();    
	}
    return 1;
}   






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
    glfwSetKeyCallback(window, key_callback); //set key event handler (callback method)

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


void setVertexBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // Bind and load vertex data using vector
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Bind and load index (element) data using vector
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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

    // Update key state
    if(action == GLFW_PRESS){
        keyStates[((char)key)] = true;
    }
    else if(action == GLFW_RELEASE){
        keyStates[((char)key)] = false;
    }
    //std::cout << (char)key << " : " << keyStates[((char)key)] << std::endl;
}