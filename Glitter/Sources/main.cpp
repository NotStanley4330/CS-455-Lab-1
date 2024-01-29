// Local Headers
#define STB_IMAGE_IMPLEMENTATION
#include "glitter.hpp"
#include "ShaderHelpers.h"
#include "ModelLoader.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

using glm::vec2;
using glm::vec3;
using glm::mat4;

void setHousePositionsColors(GLuint, vec3, vec3, vec3);

int main(int argc, char* argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);

    // **************************************
    // Setup Vertex arrays here
    // **************************************

    //Creat and bind VAO/VBO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //set house position and color data
    vec2 vertices[] = 
    { 
        vec2(-0.25f,  0.25f), 
        vec2(-0.25f,  -0.25f), 
        vec2(0.25f, -0.25f), 
        vec2(0.25f, 0.25f), 
        vec2(0.0f, 0.5f) 
    };
    vec3 colorData[] = 
    { 
        vec3(.3f, .2f, .1f), 
        vec3(.6f, .9f, .4f), 
        vec3(.5f, .8f, .3f), 
        vec3(.2f, .2f, .2f), 
        vec3(.1f, .0f, .7f) 
    };

    //Add buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colorData), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colorData), colorData);

    //load house Shaders
    GLuint houseShaders = LoadProgram("C:\\Users\\starw\\source\\repos\\NotStanley4330\\CS-455-Lab-1\\Glitter\\Shaders\\basic.vert", "C:\\Users\\starw\\source\\repos\\NotStanley4330\\CS-455-Lab-1\\Glitter\\Shaders\\basic.frag");
    glUseProgram(houseShaders);

    //set up house position and color data in the shader
    GLuint housePosition = glGetAttribLocation(houseShaders, "basicPosition");
    glEnableVertexAttribArray(housePosition);
    glVertexAttribPointer(housePosition, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), ((void*)(0)));

    GLuint houseColor = glGetAttribLocation(houseShaders, "basicColor");
    glEnableVertexAttribArray(houseColor);
    glVertexAttribPointer(houseColor, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), ((void*)(sizeof(vertices))));



    // **************************************

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // **********************************
        // Add rendering code here

        //assign the house data and draw 3 times
        setHousePositionsColors(houseShaders, vec3(-.6f, -.6f, 0.0f), vec3(1.75f, 1.75f, 0.0f), vec3(1.25f, 0.5f, 0.75f));
        glDrawArrays(GL_TRIANGLE_FAN, 0, std::size(vertices));

        setHousePositionsColors(houseShaders, vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.5f));
        glDrawArrays(GL_TRIANGLE_FAN, 0, std::size(vertices));

        setHousePositionsColors(houseShaders, vec3(0.7f, 0.25f, 0.0f), vec3(0.75f, .5f, 0.0f), vec3(.5f, 0.75f, 1.25f));
        glDrawArrays(GL_TRIANGLE_FAN, 0, std::size(vertices));

        // **********************************

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();

    return EXIT_SUCCESS;
}

//This function attaches the colors and position data to the shader
void setHousePositionsColors(GLuint houseShader, vec3 translation, vec3 scale, vec3 colorMask) 
{

    mat4 transform = glm::translate(mat4(1.0f), translation);
    transform = glm::scale(transform, scale);
    GLuint houseTransform = glGetUniformLocation(houseShader, "basicTransform");
    glUniformMatrix4fv(houseTransform, 1, GL_FALSE, glm::value_ptr(transform));

    GLuint colorTransformData = glGetUniformLocation(houseShader, "colorTransform");
    glUniform3fv(colorTransformData, 1, glm::value_ptr(colorMask));
}