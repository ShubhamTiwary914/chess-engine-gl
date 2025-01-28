#ifndef CONSTS_H
#define CONSTS_H

//Game Object Constants
const float deltaTime = 0.016f;
//Players
float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float size[] = {0.02f, 0.50f};
//p1
float pos1[] = {-0.92f, 0.0f};
char p1Keys[] = {'W', 'S'}; //(up,down) 
//p2
float pos2[] = {0.92f, 0.0f};
char p2Keys[] = {'I', 'K'}; //(up,down) 
//Ball
float center[] = { 0.0f, 0.0f };
float ballColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float radius = 0.03f;
float ballSize[] = {radius, radius};
int segments = 64;



const std::string vertexShader = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform mat4 uTransform;
void main() {
    gl_Position = uTransform * vec4(aPos, 0.0, 1.0);
})";

const std::string fragmentShader = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;
void main() {
    FragColor = uColor;
})";


#endif