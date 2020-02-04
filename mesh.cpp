#include "mesh.h"
#include "shader.h"

#include <glad/glad.h>

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

void EdgeMesh::setUpMesh(){

    //construct index array
    vector<unsigned int> indices;
    indices.reserve(length);
    for(unsigned int i=0; i<edgeVertices.size(); i+=6){
        unsigned int edge[12] = {i, i+1, i+2, i+1, i+2, i+3, i, i+2, i+4, i+1, i+3, i+5};
        indices.insert(indices.end, begin(edge), end(edge));
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO); //bind VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //set up VBO
    glBufferData(GL_ARRAY_BUFFER, edgeVertices.size()*sizeof(EdgeVertex), &edgeVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //set up EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0); //v (vertex position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)0);
    
    glEnableVertexAttribArray(1); //nv (vertex normal)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, nv));

    glEnableVertexAttribArray(2); //nA (face normal)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, nA));

    glEnableVertexAttribArray(3); //nB (face normal)
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, nB));

    glEnableVertexAttribArray(4); //kind
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(EdgeVertex), (void*)offsetof(EdgeVertex, kind));

    glBindVertexArray(0); //unbind VAO
}
