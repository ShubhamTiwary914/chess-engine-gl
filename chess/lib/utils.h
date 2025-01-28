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
#include <random>


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



#endif