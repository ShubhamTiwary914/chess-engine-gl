#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include<random>
#include "./utils.h"
#include <unordered_map>


const float velocityDelta = 1.0f;

class GameObject {
    public:
        glm::vec2 position;  // Position in world coordinates
        glm::vec2 size;      // Size of the object
        glm::vec2 velocity;  // Velocity (for ball)
        glm::vec4 color; 
        float aspect = getAspectRatio();

    void renderObject(unsigned int shaderProgram, const void* baseIndex, GLsizei indices, bool applyScale=false, bool applyAspect=false){
        glm::mat4 transform = glm::mat4(1.0f);
         //position tranlate
        transform = glm::translate(transform, glm::vec3(this->position, 0.0f));
        if(applyScale)
            transform = glm::scale(transform, glm::vec3(this->size.x, this->size.y, 1.0f));
        if(applyAspect){
            glm::mat4 aspectCorrection = glm::mat4(1.0f);
            aspectCorrection = glm::scale(aspectCorrection, glm::vec3(1.0f/aspect, 1.0f, 1.0f));
            transform = aspectCorrection * transform;
        }
        //pass transformations to uniform varibles (in the GLSL)
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uTransform"), 1, GL_FALSE, &transform[0][0]);
        glUniform4fv(glGetUniformLocation(shaderProgram, "uColor"), 1, &this->color[0]);
        //render object from indices
        glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, baseIndex);
    }
};


class PlayerRect : public GameObject {
    public:
        char controlKeys[2];

        PlayerRect(float pos[], float sz[], float col[], char controls[]){
            this->position = glm::vec2(pos[0], pos[1]);
            vectorApplyAspectTransform(this->position);
            this->size = glm::vec2(sz[0], sz[1]);
            this->color = glm::vec4(col[0], col[1], col[2], col[3]);
            this->controlKeys[0]= controls[0]; this->controlKeys[1]= controls[1];
        }

        void renderRectangle(unsigned int shaderProgram) {
            // Draw rectangle using indices 0-5
            this->renderObject(shaderProgram, (void*)0, 6, false, true);
        }

        void playerControls(float dt, std::unordered_map<char,bool> &keyStates){
            //std::cout << keyStates[controlKeys[0]] << " " << keyStates[controlKeys[1]] << std::endl;
            this->velocity = glm::vec2(0.0f,0.0f);
            if(keyStates[controlKeys[0]])
                this->velocity.y = velocityDelta;
            if(keyStates[controlKeys[1]])
                this->velocity.y = -velocityDelta;
            this->position += this->velocity * dt; 
            //std::cout << position.x << " " << position.y << std::endl;
            if(position.y - size.y/2 <= -1){
                position.y  = -0.999 + size.y/2;
            }
            else if(position.y + size.y/2 >= 1){
                position.y = 0.999 - size.y/2;
            }
        }
};


class Ball : public GameObject {
    public:
        unsigned int vertexCount; // Number of vertices for the circle

        Ball(float pos[], float sz[], float col[], unsigned int vc) {
            this->position = glm::vec2(pos[0], pos[1]);
            this->size = glm::vec2(sz[0], sz[1]);
            this->color = glm::vec4(col[0], col[1], col[2], col[3]);
            this->vertexCount = vc; 
            float velX = generateRandom(0.5, 0.8); 
            if(velX >= 0) velX = -velX;
            this->velocity.x = velX;
            this->velocity.y = generateRandom(0.5, 0.8); 
        }

        void renderBall(unsigned int shaderProgram) {
            this->renderObject(shaderProgram, (void*)(6 * sizeof(unsigned int)), (this->vertexCount) * 3, false, true);
        }

        void pongMovements(float dt, PlayerRect &p1, PlayerRect &p2){
            this->position += this->velocity * dt;
             //Walls hit
            this->boundaryCollider();
            //Player collisions
            this->playerBallCollider(p1);
            this->playerBallCollider(p2);
        }
        
    private:
        void playerBallCollider(PlayerRect &pl){
            float bL = this->position.x - this->size.x, bR = this->position.x + this->size.x;
            float bU = this->position.y + this->size.y, bD = this->position.y + this->size.y;
            //x-axes
            bool xCollide = ((bL <= pl.position.x + pl.size.x) && (bR >= pl.position.x)); 
            bool yCollide = ((bU >= pl.position.y - pl.size.y) && (bD <= pl.position.y + pl.size.y));
            if(xCollide && yCollide)
                this->velocity.x = -this->velocity.x;
        }

        //only after the whole ball has passed (not only edge)
        void boundaryCollider(){
            //top & bottom -> change direction
            if(((this->position.y - this->size.y) <= -1.0f) || (this->position.y + this->size.y >= 1.0f)){
                this->velocity.y = -this->velocity.y;
            }
            //left & right edge crossed -> end game
            if(this->position.x >= 1.0f*this->aspect|| this->position.x + this->size.x <= -1.0f*this->aspect)
                std::cout << "Game Over!" << std::endl;
        }
};




#endif