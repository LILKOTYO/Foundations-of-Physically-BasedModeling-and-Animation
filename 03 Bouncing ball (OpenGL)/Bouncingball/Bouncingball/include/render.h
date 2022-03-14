#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/simulator.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const int screenWidth = 800;
const int screenHeight = 600;
float fov = 45.0;

float d_air = 0.3f;
float d_wall = 0.0f;
float bounce = 0.7f;

void render(Ball ball, Box box)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Bouncing Ball", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Vertex Shader
    Shader ourShader(".\\src\\shader.vs", ".\\src\\shader.fs");

    float vertices[] = {
        ball.m_pos.x, ball.m_pos.y
    };


    // Store triangle vertex data in Vertex Buffer Object 
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Create an Vertex Array Object to store the vertex attribute
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //set up vertex data (and buffer(s)) and configure vertex attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    ourShader.use();

    glEnable(GL_DEPTH_TEST);

    //Render loop
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        simulation(ball, box, d_air, d_wall, bounce);
        std::cout << ball.m_pos.x << " , " << ball.m_pos.y << std::endl;
        if (CheckStop(ball, box))
        {
            system("pause");
            return;
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.use();
        //
        glBindVertexArray(VAO);
        int posLoc = glGetUniformLocation(ourShader.ID, "pos");
        glUniform2f(posLoc, ball.m_pos.x - 20.0f, ball.m_pos.y - 20.0f);
        glDrawArrays(GL_POINTS, 0, 1);
        

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
