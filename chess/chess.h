//gl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//libs
#include "glcontrol.h"
#include "lib/utils.h"
#include "lib/consts.h"
#include "lib/shaders.h"
#include "lib/_render.h"
//standard
#include <vector>



const float BG_COLOR[] = {0.0, 0.0, 0.0};
const int WIDTH = 800;
const int HEIGHT = 600;
const char* TITLE = "Chess MasterGL";



// Single cell vertex data (normalized square)
std::vector<float> cellVertices = {
    // positions
    -0.5f, -0.5f, 0.0f,  // bottom left
     0.5f, -0.5f, 0.0f,  // bottom right
     0.5f,  0.5f, 0.0f,  // top right
    -0.5f,  0.5f, 0.0f   // top left
};

// Indices for drawing squares with triangles
std::vector<unsigned int> indices = {
    0, 1, 2,  // first triangle
    0, 2, 3   // second triangle
};

// Generate grid positions for instancing (8x8 grid)
std::vector<glm::vec2> gridPositions;
void generateGridPositions() {
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            gridPositions.push_back(glm::vec2(col, 7-row)); // Flip rows to match chess coordinates
        }
    }
}


int game(){
    GLFWwindow* window = init(WIDTH, HEIGHT, TITLE);
    if(window == nullptr) return -1;
    glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //> Vertex, Shader & objects
    Shader shader = Shader(vertexShaderSource, fragmentShaderSource);
    generateGridPositions();
    unsigned int VBO, instanceVBO, EBO, VAO;
    setVertexAttributes(VBO, instanceVBO, EBO, VAO, cellVertices, indices, gridPositions);
    RenderBoard board(VAO, shader);

    // > Event loop
    while(!glfwWindowShouldClose(window)){
        glClearColor(BG_COLOR[0], BG_COLOR[1], BG_COLOR[2], 0.2f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update aspect ratio if window is resized
        board.updateAspectRatio();
        
        // Render chess board
        board.render();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Clear memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}



