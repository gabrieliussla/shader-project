#ifndef MESH
#define MESH

#include "shader.h"

#include <glm/glm.hpp>
#include <vector>

using namespace std;

struct ScreenVertex{
    glm::vec2 position;
    glm::vec2 texCoord;
};

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
};

struct EdgeVertex{
    glm::vec3 v;    //vertex position
    glm::vec3 v2;   //other vertex position
    glm::vec3 nv;   //vertex normal
    glm::vec3 nA;   //face A normal
    glm::vec3 nB;   //face B normal
    short kind;     //what kind of vertex it is (outer:0, inner:1, cap:2)
};

struct KeyVertex{
    glm::vec3 position;
    glm::vec3 normal;
    float lightness;
    short kind;
};

class Mesh{
    public:
        void draw();
    protected:
        unsigned int VAO, VBO, EBO;
        unsigned int length;
        void setUpMesh();
};

class ScreenMesh : public Mesh{
    public:
        vector<ScreenVertex> screenVertices;
        vector<unsigned int> indices;
        ScreenMesh(vector<ScreenVertex> &vertices, vector<unsigned int> &indices);
    protected:
        void setUpMesh();
};

class SimpleMesh : public Mesh{
    public:
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        SimpleMesh(vector<Vertex> &vertices, vector<unsigned int> &indeces);
    protected:
        void setUpMesh();
};

class EdgeMesh : public Mesh{
    public:
        vector<EdgeVertex> edgeVertices;
        EdgeMesh(vector<EdgeVertex> &edgeVertices);
    protected:
        void setUpMesh();
};

class TextureMesh : public Mesh{
    public:
        vector<KeyVertex> keyVertices;
        TextureMesh(vector<KeyVertex> &keyVertices);
    protected:
        void setUpMesh();
};

#endif
