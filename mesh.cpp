#include "mesh.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/gtx/string_cast.hpp> //for testing

#include <vector>
#include <iostream>
using namespace std;

SimpleMesh::SimpleMesh(vector<Vertex> vertices, vector<unsigned int> indices){
    this->vertices = vertices;
    this->indices  = indices;
    this->length   = indices.size();
    setUpMesh();
}

EdgeMesh::EdgeMesh(vector<EdgeVertex> edgeVertices){
    this->edgeVertices = edgeVertices;
    this->length       = edgeVertices.size() * 2;
    setUpMesh();
}

void Mesh::draw(Shader shader){
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, length, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setUpMesh(){
    cout << "Mesh set-up unimplemented";
}

void SimpleMesh::setUpMesh(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO); //bind VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //set up VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //set up EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0); //vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    glEnableVertexAttribArray(1); //normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0); //unbind VAO
}

vector<EdgeVertex> addtovec(vector<EdgeVertex> vector, glm::vec3 v, glm::vec3 nv, glm::vec3 nA, glm::vec3 nB, short kind){
    EdgeVertex e;
    e.v = v;
    e.nv = nv;
    e.nA = nA;
    e.nB = nB;
    e.kind = kind;
    vector.push_back(e);
    return vector;
}

void EdgeMesh::setUpMesh(){

    //construct index array
    vector<unsigned int> indices;
    indices.reserve(length);
    for(unsigned int i=0; i<edgeVertices.size(); i+=6){
        unsigned int edge[12] = {i, i+2, i+1, i, i+1, i+3, i, i+4, i+2, i+1, i+5, i+3};
        indices.insert(indices.end(), begin(edge), end(edge));
    }
    

    // TEST ////////////////////
    //vector<EdgeVertex> vec(edgeVertices.begin(), edgeVertices.begin()+6);
    //edgeVertices = vec;
    //vector<unsigned int> newVec(indices.begin(), indices.begin()+6);
    //indices = newVec;
    //length= 6;
    /*
    edgeVertices.clear();
    edgeVertices=addtovec(edgeVertices,glm::vec3(1.496475, 0.127575, 0.000000),glm::vec3(-0.396698, -0.916073, 0.058660),glm::vec3(-0.574199, -0.738601, 0.353218),glm::vec3(-0.512164, -0.738790, -0.438039),0);
    edgeVertices=addtovec(edgeVertices,glm::vec3(1.500000, 0.150000, 0.000000),glm::vec3(-0.261776, -0.533038, 0.804577),glm::vec3(-0.574199, -0.738601, 0.353218),glm::vec3(-0.512164, -0.738790, -0.438039),0);
    edgeVertices=addtovec(edgeVertices,glm::vec3(1.496475, 0.127575, 0.000000),glm::vec3(-0.396698, -0.916073, 0.058660),glm::vec3(-0.574199, -0.738601, 0.353218),glm::vec3(-0.512164, -0.738790, -0.438039),1);
    edgeVertices=addtovec(edgeVertices,glm::vec3(1.500000, 0.150000, 0.000000),glm::vec3(-0.261776, -0.533038, 0.804577),glm::vec3(-0.574199, -0.738601, 0.353218),glm::vec3(-0.512164, -0.738790, -0.438039),1);
    edgeVertices=addtovec(edgeVertices,glm::vec3(1.496475, 0.127575, 0.000000),glm::vec3(-0.396698, -0.916073, 0.058660),glm::vec3(-0.574199, -0.738601, 0.353218),glm::vec3(-0.512164, -0.738790, -0.438039),2);
    edgeVertices=addtovec(edgeVertices,glm::vec3(1.500000, 0.150000, 0.000000),glm::vec3(-0.261776, -0.533038, 0.804577),glm::vec3(-0.574199, -0.738601, 0.353218),glm::vec3(-0.512164, -0.738790, -0.438039),2);
    
    indices.clear();
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(4);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(5);

    length = 12;
*/
    //for(EdgeVertex v : edgeVertices){
    //    cout << to_string(v.v) << "\t" << to_string(v.nv) << "\t" << to_string(v.nA) << "\t" << to_string(v.nB) << "\t" << v.kind << "\n";
    //}
    //for(unsigned int i : indices){
    //    cout << i;
    //}
    //cout << "\n";

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO); //bind VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //set up VBO
    glBufferData(GL_ARRAY_BUFFER, edgeVertices.size()*sizeof(EdgeVertex), &edgeVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //set up EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0); //v
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)0);
    
    glEnableVertexAttribArray(1); //v2
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, v2));

    glEnableVertexAttribArray(2); //nv
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, nv));

    glEnableVertexAttribArray(3); //nA
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, nA));

    glEnableVertexAttribArray(4); //nB
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, nB));

    glEnableVertexAttribArray(5); //kind
    glVertexAttribIPointer(5, 1, GL_SHORT, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, kind));

    glBindVertexArray(0); //unbind VAO
}
