#define DEV_MODE false
#define WIDTH 800
#define HEIGHT 600

//gl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//libs
//gui API
#include "glcontrol.h"
#include "lib/utils/glutils.h"
#include "lib/shaders.h"
#include "lib/shaders.h"
#include "lib/_render.h"
#include "lib/stb_image.h"
//engine
#include "lib/_enginemain.h"
//standard
#include <vector>



const float BG_COLOR[] = {0.0, 0.0, 0.0};
const char* TITLE = "Chess MasterGL";


// Single cell vertex data (normalized square)
std::vector<float> cellVertices = {
    // positions             //tex coords
    // bottom left 
    -0.49f, -0.49f, 0.0f,     0.0f, 0.0f,
    // bottom right
     0.49f, -0.49f, 0.0f,     1.0f, 0.0f, 
    // top right
    0.49f,  0.49f, 0.0f,     1.0f, 1.0f,
    // top left
    -0.49f,  0.49f, 0.0f,     0.0f, 1.0f
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




void eventLoop(GLFWwindow *window, RenderBoard &board, RenderPieces &pieces){
    while(!glfwWindowShouldClose(window)){
        glClearColor(BG_COLOR[0], BG_COLOR[1], BG_COLOR[2], 0.2f);
        glClear(GL_COLOR_BUFFER_BIT);
        //resize handler
        board.updateAspectRatio();
        pieces.updateAspectRatio();
        //> game objects render
        //main game code ================================================
        board.render();
        //piece render (map piece set)
        for(int rank=8; rank>=1; rank--){
            for(int file=65; file<= 72; file++){
                char pieceKey = mainboard.getPiece((char)file, rank);
                if(pieceKey != 'E')
                    pieces.render(pieceKey, (char)file, rank);
            }
        }
        // ================================================

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}




int gameStart(){
    if(DEV_MODE)
        return chessEngine();
        
    GLFWwindow* window = init(TITLE);
    if(window == nullptr) return -1;
    glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //> Vertex, Shader & objects
    // Board setup
    Shader boardShader = Shader(boardVertexShader, boardFragmentShader);
    generateGridPositions();
    unsigned int VBO, instanceVBO, EBO, VAO;
    setVertexAttributes(VBO, instanceVBO, EBO, VAO, cellVertices, indices, gridPositions);
    RenderBoard board(VAO, boardShader);

    // Piece setup
    Shader pieceShader = Shader(pieceVertexShader, pieceFragmentShader);
    loadPieceTextures();  // Load all piece textures
    RenderPieces pieces(VAO, pieceShader);

    //>Game loop & Init.
    initializeBoard();
    eventLoop(window, board, pieces);

    //Clear memory
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}