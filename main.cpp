#include <iostream>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp> // for testing

#include "shader.h"
#include "model.h"


//----- Globals -----//

int MODE = 1;

int screenWidth = 600;
int screenHeight = 600;

float deltaTime;
float lastFrameTime;

glm::vec3 cameraPos = glm::vec3(6.9,1.2,8.6);
glm::vec3 cameraFront = glm::vec3(-0.5,0,-0.8);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float cameraYaw = -0.56;


//----- Callback functions -----//

// On size change
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
}

// On input
bool processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = deltaTime * 4;
    float cameraTurnSpeed = deltaTime * 2;
    bool move = false;
    bool turn = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront; move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront; move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cameraPos += cameraUp * cameraSpeed; move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraPos -= cameraUp * cameraSpeed; move = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraYaw -= cameraTurnSpeed; turn = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cameraYaw += cameraTurnSpeed; turn = true;
    }

    if (turn) {
        cameraFront.x = sin(cameraYaw);
        cameraFront.z = -cos(cameraYaw);
    }

    return move || turn;
}




glm::vec2 screen(glm::mat4 projection, glm::vec3 vector){
    glm::vec4 proj_vector = projection * glm::vec4(vector, 1.0);
    return glm::vec2(proj_vector.x/proj_vector.w, proj_vector.y/proj_vector.w);
}

//----- Main Function -----//

int main()
{

    //----- Initialise libraries and view window -----//

    // Initialise glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a Windows window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "GLTest", NULL, NULL);
    if (window == NULL) {
        std::cout << "Creating window failed!";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load the GLAD library
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Loading GLAD failed!";
        return -1;
    }

    // Set up the callback function when window size changes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Create a viewport to span the whole window
    glViewport(0, 0, screenWidth, screenHeight);


    //----- Set Up -----//

    // Compile shaders
    string name;
    switch(MODE){
        case  0: name = "simple";   break;
        case  1: name = "edge";     break;
    }
    Shader shaders(("shaders/"+name+".vert").c_str(), ("shaders/"+name+".frag").c_str());

    // Setup model
    Model fish("models/fish.obj", MODE);
    
    // Enable shaders
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    shaders.use();
    
    // Set up static matrices
    glm::mat4 model;

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    shaders.setMat4((char*)"view", view);

    glm::mat4 projection = glm::perspective(1.2f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
    shaders.setMat4((char*)"projection", projection);
    
    // Background colour
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);


    //----- Render Loop -----//

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (processInput(window)) {
            view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
            shaders.setMat4((char*)"view", view);
            //cout << to_string(projection * view * model * glm::vec4(1.4, 2.4, 0.0, 1.0)) << "\n";
        }

        //changes that depend on time
        float timeValue = (float)glfwGetTime();
        deltaTime = timeValue - lastFrameTime;
        lastFrameTime = timeValue;

        float rotationValue = 0;(sin(timeValue*2 + cos(timeValue*2)) - 1) * 0.4;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, -0.4, 0.0));
        model = glm::rotate(model, rotationValue, glm::vec3(0.0, 0.0, 1.0));
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        shaders.setMat4((char*)"model", model);

        //draw the triangles
        fish.draw(shaders);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
