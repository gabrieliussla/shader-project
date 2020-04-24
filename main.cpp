#include <iostream>
#include <string>
#include <vector>
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
#include "mesh.h"

#define SAMPLES 4
#define BRUSH "splat"
#define TEXTURE "paper_normals"

struct Object{
    SimpleModel  *fill;
    EdgeModel    *edges;
    TextureModel *texture;
    glm::mat4 modelMat;
    glm::vec3 colour;
};

//----- Globals -----//

int screenWidth = 600;
int screenHeight = 600;
int screenChange = 1;

float startTime;
float deltaTime;
float lastFrameTime;
long totalFrames = 0;

glm::vec3 light = glm::vec3(0.0, 8.0, 0.0);
glm::vec3 cameraPos = glm::vec3(-1.4, 2.9, -2.2);//glm::vec3(4.2, 3.2, 6.2);
glm::vec3 cameraFront = glm::vec3(0.5, 0.0, 0.8);//glm::vec3(-0.5,0,-0.8);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float cameraYaw = 2.56;
int viewChange = 1;

//----- Callback functions -----//

// On size change
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
    screenChange = 1;
}

// On input
bool processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
        if(lastFrameTime-startTime > 1) cout << "FPS: " << ((float)totalFrames/(lastFrameTime-startTime)) << "   Frame Time: " << ((lastFrameTime-startTime)/(float)totalFrames) << '\n';
        totalFrames = 0;
        startTime = (float)glfwGetTime();
    }

    float cameraSpeed = deltaTime * 3;
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

//----- Other functions -----//

struct Object *newObject(string name, glm::mat4 model, glm::vec3 colour, float density){
    struct Object *obj = new Object;
    obj->fill = new SimpleModel("models/"+name);
    obj->edges = new EdgeModel("models/"+name);
    obj->texture = new TextureModel("models/"+name, density);
    obj->modelMat = model;
    obj->colour = colour;
    return obj;
}

void deleteObject(struct Object *obj){
    delete obj->fill;
    delete obj->edges;
    delete obj->texture;
    delete obj;
}

glm::vec3 screen(glm::mat4 projection, glm::vec3 vector){
    glm::vec4 proj_vector = projection * glm::vec4(vector, 1.0);
    return glm::vec3(glm::vec2(proj_vector) / glm::abs(proj_vector.w), proj_vector.z);
}

glm::vec3 genPaperLight(glm::mat4 projection, glm::vec3 light){
    glm::vec3 paperLight = screen(projection, light);
    paperLight.z = 3;
    if(paperLight.x >  2) paperLight.x =  2;
    if(paperLight.x < -2) paperLight.x = -2;
    if(paperLight.y >  2) paperLight.y =  2;
    if(paperLight.y < -2) paperLight.y = -2;
    return paperLight;
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
    glfwWindowHint(GLFW_SAMPLES, SAMPLES);
    
    glfwSwapInterval(0.0001);
    // Create a Windows window
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "GLTest", NULL, NULL);
    if (window == NULL) {
        std::cout << "Creating window failed!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load the GLAD library
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Loading GLAD failed!\n";
        return -1;
    }

    // Set up the callback function when window size changes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Create a viewport to span the whole window
    glViewport(0, 0, screenWidth, screenHeight);

    //----- Set Up -----//
    
    // Compile shaders
    Shader fill("shaders/fill.vert", "shaders/fill.frag");
    Shader edges("shaders/edge.vert", "shaders/edge.frag");
    Shader textures("shaders/texture.vert", "shaders/texture.frag");
    Shader image("shaders/image.vert", "shaders/imagefx.frag");

    // Set up all models
    std::vector<struct Object *> objects;
    //objects.push_back(newObject("body.obj", glm::scale(glm::mat4(1.0f), glm::vec3(0.03)), 0.02));
    //objects.push_back(newObject("elk.obj", glm::scale(glm::mat4(1.0f), glm::vec3(0.03)), 0.01));
    objects.push_back(newObject("tree.obj", glm::scale(glm::mat4(1.0f), glm::vec3(1.6)), glm::vec3(0.61,0.54,0.43), 39));
    //objects.push_back(newObject("tree1.obj", glm::scale(glm::mat4(1.0f), glm::vec3(0.6)), 3));
    //objects.push_back(newObject("cup.obj", glm::rotate(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.1, 0.0)), glm::vec3(0.04)), -1.57f, glm::vec3(1, 0, 0)), 0.01));
    objects.push_back(newObject("sphere.obj", glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.7f, -4.2f)), glm::vec3(0.9, 0.8, 0.2), 9.0));
    // Create a mesh for the screen quad
    vector<ScreenVertex> screenVertices = {
        {glm::vec2(-1.0, 1.0),glm::vec2(0.0,1.0)},
        {glm::vec2( 1.0, 1.0),glm::vec2(1.0,1.0)},
        {glm::vec2(-1.0,-1.0),glm::vec2(0.0,0.0)},
        {glm::vec2( 1.0,-1.0),glm::vec2(1.0,0.0)}};
    vector<unsigned int> screenIndices  = {0, 1, 2, 1, 3, 2};
    ScreenMesh screenMesh(screenVertices, screenIndices);

    // Set up the multisampled framebuffer to render to
    unsigned int screenTexMS;
    glGenTextures(1, &screenTexMS);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, screenTexMS);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLES, GL_RGB, screenWidth, screenHeight, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    unsigned int rboMS;
    glGenRenderbuffers(1, &rboMS);
    glBindRenderbuffer(GL_RENDERBUFFER, rboMS);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    unsigned int framebufferMS;
    glGenFramebuffers(1, &framebufferMS);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferMS);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, screenTexMS, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboMS);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "Setting up multisample frame buffer failed!\n";
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Set up the framebuffer to blit to
    glActiveTexture(GL_TEXTURE0);
    unsigned int screenTex;
    glGenTextures(1, &screenTex);
    glBindTexture(GL_TEXTURE_2D, screenTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTex, 0);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        std::cout << "Setting up frame buffer failed!\n";
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Get brush textures
    int width, height, nrChannels;
    unsigned char *data = stbi_load(("textures/"+std::string(BRUSH)+".png").c_str(), &width, &height, &nrChannels, 0);
    if(!data){
        std::cout << "Loading brush texture failed!\n";
        return -1;
    }
    glActiveTexture(GL_TEXTURE0);
    unsigned int brush;
    glGenTextures(1, &brush);
    glBindTexture(GL_TEXTURE_2D, brush);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    // Load paper texture
    data = stbi_load(("textures/"+std::string(TEXTURE)+".jpg").c_str(), &width, &height, &nrChannels, 0);
    if(!data){
        std::cout << "Loading texture failed!\n";
        return -1;
    }
    glActiveTexture(GL_TEXTURE1);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    // Buffer settings
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Set up uniforms
    float ratio = (float)screenWidth/(float)screenHeight;
    glm::mat4 model;
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(1.2f, ratio, 0.1f, 100.0f);
    glm::vec3 paperLight = genPaperLight(projection*view, light);

    fill.use();
    fill.setVec3((char*)"light", light);
    edges.use();
    edges.setVec3((char*)"light", light);
    image.use();
    image.setVec3((char*)"light", light);
    image.setInt((char*)"paper", 1);

    //----- Render Loop -----//
    
    startTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        //if camera has moved, update the varialbes
        if (processInput(window)) {
            viewChange = 1;
            view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
            paperLight = genPaperLight(projection*view, light);
            //cout << to_string(cameraPos) << "\t" << to_string(cameraFront) << "\n";
        }
        //if window size has changed, set according properties
        if(screenChange){
            ratio = (float)screenWidth/(float)screenHeight;
            projection = glm::perspective(1.2f, ratio, 0.1f, 100.0f);

            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, screenTexMS);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SAMPLES, GL_RGB, screenWidth, screenHeight, GL_TRUE);
            glBindTexture(GL_TEXTURE_2D, screenTex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindRenderbuffer(GL_RENDERBUFFER, rboMS);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, SAMPLES, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
        }

        //changes that depend on time
        float timeValue = (float)glfwGetTime();
        deltaTime = timeValue - lastFrameTime;
        lastFrameTime = timeValue;
        float rotationValue = 0;//(sin(timeValue*2 + cos(timeValue*2)) - 1) * 0.4;
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1));
        model = glm::translate(model, glm::vec3(0.0, -0.4, 0.0));
        model = glm::rotate(model, rotationValue, glm::vec3(0.0, 0.0, 1.0));
        model = glm::translate(model, glm::vec3(0.0, 0.5, 0.0));
        //shaders.setVec3((char*)"light", glm::vec3(5*sin(timeValue), 5.0, 5*cos(timeValue)));


        glBindFramebuffer(GL_FRAMEBUFFER, framebufferMS);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Draw the fill
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        fill.use();
        if(viewChange)
            fill.setMat4((char*)"view", view);
        if(screenChange)
            fill.setMat4((char*)"projection", projection);
        for(int i=0; i<objects.size(); i++){
            fill.setMat4((char*)"model", objects[i]->modelMat);
            fill.setVec3((char*)"shade", objects[i]->colour);
            glStencilFunc(GL_ALWAYS, i+1, 0xFF);
            objects[i]->fill->draw();
        }
        
        // Draw the edges
        glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
        edges.use();
        if(viewChange){
            edges.setMat4((char*)"view", view);
            edges.setVec3((char*)"eye", cameraPos);}
        if(screenChange){
            edges.setMat4((char*)"projection", projection);
            edges.setFloat((char*)"ratio", ratio);}
        for(int i=0; i<objects.size(); i++){
            edges.setMat4((char*)"model", objects[i]->modelMat);
            objects[i]->edges->draw();
        }

        // Draw the texture
        glDisable(GL_DEPTH_TEST);
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brush);
        textures.use();
        if(viewChange){
            textures.setMat4((char*)"view", view);
            textures.setVec3((char*)"eye", cameraPos);}
        if(screenChange)
            textures.setMat4((char*)"projection", projection);
        for(int i=0; i<objects.size(); i++){
            textures.setMat4((char*)"model", objects[i]->modelMat);
            glStencilFunc(GL_EQUAL, i+1, 0xFF);
            objects[i]->texture->draw();
        }
       
        // Draw effects
        //blit to the non-multisample buffer so it can be displayed
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferMS);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
        glDrawBuffer(GL_BACK);
        glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        //draw quad to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, screenTex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        image.use();
        if(viewChange){
            image.setVec3((char*)"flatlight", paperLight);
            viewChange=0;}
        if(screenChange){
            image.setFloat((char*)"ratio", ratio);
            screenChange=0;}
        screenMesh.draw();

        //finish frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        totalFrames ++;
    }

    glDeleteBuffers(1, &framebuffer);
    glDeleteBuffers(1, &framebufferMS);
    glfwTerminate();
    return 0;
}
