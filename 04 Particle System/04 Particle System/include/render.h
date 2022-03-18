#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader.h>
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "particle.h"
#include "pgenerator.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

const int screenWidth = 800;
const int screenHeight = 600;
float fov = 45.0;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float lastX, lastY;
float yaw = -90.0f;
float pitch = 0.0f;

void render(ppointgenerator& generator, std::vector<shared_ptr<particle>>& p_list)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Vertex Shader
    Shader ourShader(".\\src\\shader.vs", ".\\src\\shader.fs");
    //Shader ourShader(".\\src\\shader.vs", ".\\src\\exercise1.fs");

    float vertices[3] = {0.0, 0.0, 0.0};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindVertexArray(0);

    //ourShader.use();

    glEnable(GL_DEPTH_TEST);

    //Render loop
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (!glfwWindowShouldClose(window))
    {
        if (p_list.size() < p_count)
        {
            generator.generateDir(p_list, 3, 100000, 0.5, 0.1 * pi);
            //generator.generateOmni(p_list, 3, 100000, 0.5);
        }
        int size = p_list.size();
        processInput(window);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(background.x, background.y, background.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);

        for (auto& p : p_list)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, p->pos);
            int modelLoc = glGetUniformLocation(ourShader.ID, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glm::mat4 view;
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            int viewLoc = glGetUniformLocation(ourShader.ID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

            int colorLoc = glGetUniformLocation(ourShader.ID, "aColor");
            glUniform3f(colorLoc, p->color.x, p->color.y, p->color.z);

            glDrawArrays(GL_POINTS, 0, 1);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        for (shared_ptr<particle> p : p_list)
        {
            p->update();
        }

        generator.check(p_list);
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

    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}