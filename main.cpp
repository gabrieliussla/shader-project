#include <iostream>
#include <string>
#include <filesystem>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp> // for testing

#include "shader.h"
#include "model.h"

#define BRUSH "hatch"

//----- Globals -----//

int screenWidth = 600;
int screenHeight = 600;

float deltaTime;
float lastFrameTime;

glm::vec3 cameraPos = glm::vec3(-1.4, 2.9, -2.2);//glm::vec3(4.2, 3.2, 6.2);
glm::vec3 cameraFront = glm::vec3(0.5, 0.0, 0.8);//glm::vec3(-0.5,0,-0.8);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float cameraYaw = 2.56;


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

    float cameraSpeed = deltaTime * 2;
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
    //Shader shader(("shaders/"+name+".vert").c_str(), ("shaders/"+name+".frag").c_str());
    Shader fill("shaders/fill.vert", "shaders/fill.frag");
    Shader edges("shaders/edge.vert", "shaders/edge.frag");
    Shader textures("shaders/texture.vert", "shaders/texture.frag");

    // Setup model
    Model potFill("models/pot.obj", 0);
    Model potEdges("models/pot.obj", 1);
    Model potTexture("models/pot.obj", 2);
    Model sphereFill("models/sphere.obj", 0);
    Model sphereEdges("models/sphere.obj", 1);
    Model sphereTexture("models/sphere.obj", 2);

    // Get textures
    int width, height, nrChannels;
    unsigned char *data = stbi_load(("textures/"+std::string(BRUSH)+".png").c_str(), &width, &height, &nrChannels, 0);
    if(!data){
        std::cout << "Loading texture failed!";
        return -1;
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    //activate texture, might need to change if using multiple
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Buffer settings
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up uniforms
    glm::mat4 model;
    glm::mat4 sphereModel = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.7f, -4.2f));
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(1.2f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
    
    edges.use();
    edges.setMat4((char*)"view", view);
    edges.setVec3((char*)"eye", cameraPos);
    edges.setVec3((char*)"light", glm::vec3(0.0, 8.0, 0.0));
    edges.setMat4((char*)"projection", projection);
    
    fill.use();
    fill.setMat4((char*)"view", view);
    fill.setMat4((char*)"projection", projection);

    textures.use();
    textures.setMat4((char*)"view", view);
    textures.setVec3((char*)"eye", cameraPos);
    textures.setMat4((char*)"projection", projection);


    // Background colour
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


    //----- Render Loop -----//

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (processInput(window)) {
            view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
            edges.use();
            edges.setMat4((char*)"view", view);
            edges.setVec3((char*)"eye", cameraPos);
            fill.use();
            fill.setMat4((char*)"view", view);
            textures.use();
            textures.setMat4((char*)"view", view);
            textures.setVec3((char*)"eye", cameraPos);
            //cout << to_string(projection * view * model * glm::vec4(1.4, 2.4, 0.0, 1.0)) << "\n";
            //cout << to_string(cameraPos) << "\t" << to_string(cameraFront) << "\n";
        }

        //changes that depend on time
        float timeValue = (float)glfwGetTime();
        deltaTime = timeValue - lastFrameTime;
        lastFrameTime = timeValue;

        float rotationValue = 0;//(sin(timeValue*2 + cos(timeValue*2)) - 1) * 0.4;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, -0.4, 0.0));
        model = glm::rotate(model, rotationValue, glm::vec3(0.0, 0.0, 1.0));
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));

        //shaders.setVec3((char*)"light", glm::vec3(5*sin(timeValue), 5.0, 5*cos(timeValue)));

        //draw the fill
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        fill.use();
        fill.setMat4((char*)"model", model);
        potFill.draw(fill);
        
        glStencilFunc(GL_ALWAYS, 2, 0xFF);
        fill.setMat4((char*)"model", sphereModel);
        sphereFill.draw(fill);
        
        //draw the edges
        glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
        edges.use();
        edges.setMat4((char*)"model", model);
        potEdges.draw(edges);
        edges.setMat4((char*)"model", sphereModel);
        sphereEdges.draw(edges);

        //draw the texture
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        textures.use();
        textures.setMat4((char*)"model", model);
        potTexture.draw(textures);

        glStencilFunc(GL_EQUAL, 2, 0xFF);
        textures.setMat4((char*)"model", sphereModel);
        sphereTexture.draw(textures);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
