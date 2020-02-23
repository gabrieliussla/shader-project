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

void Mesh::draw(){
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

vector<EdgeVertex> addtovec(vector<EdgeVertex> vector, glm::vec3 v, glm::vec3 v2, glm::vec3 nv, glm::vec3 nA, glm::vec3 nB, short kind){ //TESTING
    EdgeVertex e;
    e.v = v;
    e.v2 = v2;
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
        unsigned int edge[12] = {i, i+2, i+3, i, i+3, i+1, i, i+4, i+2, i+1, i+3, i+5};
        indices.insert(indices.end(), begin(edge), end(edge));
    }
    

    // TEST ////////////////////
    //vector<EdgeVertex> vec(edgeVertices.begin()+12000, edgeVertices.begin()+17000);
    //edgeVertices = vec;
    //vector<unsigned int> newVec(indices.begin(), indices.begin()+6000);
    //indices = newVec;
    //length= 3000;
    
    //edgeVertices.clear();
    //edgeVertices=addtovec(edgeVertices,glm::vec3(0.0, 0.0, 0.0),glm::vec3(1.0, 1.0, 0.0),glm::vec3(-0.8, 0.2, 0.0),glm::vec3(-0.5, -0.7, 0.3),glm::vec3(-0.5, -0.7, -0.4),0);
    //edgeVertices=addtovec(edgeVertices,glm::vec3(1.0, 1.0, 0.0),glm::vec3(0.0, 0.0, 0.0),glm::vec3( 0.2, 0.8, 0.0),glm::vec3(-0.5, -0.7, 0.3),glm::vec3(-0.5, -0.7, -0.4),0);
    //edgeVertices=addtovec(edgeVertices,glm::vec3(0.0, 0.0, 0.0),glm::vec3(1.0, 1.0, 0.0),glm::vec3(-0.8, 0.2, 0.0),glm::vec3(-0.5, -0.7, 0.3),glm::vec3(-0.5, -0.7, -0.4),1);
    //edgeVertices=addtovec(edgeVertices,glm::vec3(1.0, 1.0, 0.0),glm::vec3(0.0, 0.0, 0.0),glm::vec3( 0.2, 0.8, 0.0),glm::vec3(-0.5, -0.7, 0.3),glm::vec3(-0.5, -0.7, -0.4),1);
    //edgeVertices=addtovec(edgeVertices,glm::vec3(0.0, 0.0, 0.0),glm::vec3(1.0, 1.0, 0.0),glm::vec3(-0.8, 0.2, 0.0),glm::vec3(-0.5, -0.7, 0.3),glm::vec3(-0.5, -0.7, -0.4),2);
    //edgeVertices=addtovec(edgeVertices,glm::vec3(1.0, 1.0, 0.0),glm::vec3(0.0, 0.0, 0.0),glm::vec3( 0.2, 0.8, 0.0),glm::vec3(-0.5, -0.7, 0.3),glm::vec3(-0.5, -0.7, -0.4),2);

    //length = 12;

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
