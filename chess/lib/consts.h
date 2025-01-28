#ifndef CONSTS_H
#define CONSTS_H

#include <string>



// Shader sources -> render board
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aOffset;
    uniform float aspectRatio;
    flat out vec2 gridPos;
    
    void main() {
        gridPos = aOffset;
        
        // Convert 8x8 grid coordinates to [-1,1] range
        float x = (aOffset.x + aPos.x) / 4.0 - 1.0;  
        float y = (aOffset.y + aPos.y) / 4.0 - 1.0;
        
        y = y + 0.11;
        x = x / aspectRatio; //adjust for aspect ratio (only x )
        gl_Position = vec4(x, y, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
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


#endif