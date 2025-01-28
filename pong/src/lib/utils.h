/**
  //> Static Methods for Utilities (custom transformations/calculations)
*/
#ifndef UTILS_H
#define UTILS_H

//Gl frameworks
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>


void appendCircle(std::vector<float>& vertices, std::vector<unsigned int>& indices, float centerX, float centerY, float radius, int segments) {
    unsigned int startIndex = vertices.size() / 2; // Calculate the starting index for the circle vertices
    vertices.push_back(centerX);                  // Center of the circle
    vertices.push_back(centerY);

    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * i / segments; // Angle for this vertex
        float x = centerX + radius * cos(angle); // X coordinate
        float y = centerY + radius * sin(angle); // Y coordinate
        vertices.push_back(x);
        vertices.push_back(y);

        // Create indices for the triangle fan
        if (i > 0) {
            indices.push_back(startIndex);       // Center of the circle
            indices.push_back(startIndex + i);  // Current vertex
            indices.push_back(startIndex + i + 1); // Next vertex
        }
    }

    // Close the circle by connecting the last vertex to the first
    indices.push_back(startIndex);
    indices.push_back(startIndex + segments);
    indices.push_back(startIndex + 1);
}

float getAspectRatio(){
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    float aspect = (float)width / (float)height;
    return aspect;
}

//Random number in range:  [-a.-b]  or [a,b] (.3 decimal places precision float)
double generateRandom(double a, double b) {
    // Random device and generator
    std::random_device rd;
    std::mt19937 gen(rd());
    //distributions for both ranges
    std::uniform_real_distribution<> dist1(-a, -b);
    std::uniform_real_distribution<> dist2(a, b);
    // either +ve or +ve
    std::uniform_int_distribution<> rangeChooser(0, 1);
    int choice = rangeChooser(gen);
    // Generate a random number in the chosen range
    return ((choice == 0) ? (dist1(gen)) : (dist2(gen)));
}

//Transform vector to the aspect ratio -> to normalize according to device width/height 
void vectorApplyAspectTransform(glm::vec2 &vec){
    float aspectRatio = getAspectRatio();
    vec.x *= aspectRatio;
    vec.y *= aspectRatio;
}


#endif