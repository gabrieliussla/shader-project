#include "model.h"

#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp> //for testing
#include <iostream>
#include <string>
#include <unordered_map>
#include <tuple>

struct edge_hash{
    size_t operator()(tuple<glm::vec3, glm::vec3> x) const {
        return hash<glm::vec3>()(get<1>(x)) ^ hash<glm::vec3>()(get<0>(x));
    }
};

glm::vec3 to_vec3(aiMesh *mesh, aiVector3D v){
    glm::vec3 vector = glm::vec3(v.x, v.y, v.z);
    return vector;
}

Model::Model(string path, int mode){
    // Load model using assimp
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if(!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE){
        std::cout << "Loading scene failed!";
        return;
    }
    dir = path.substr(0, path.find_last_of('/'));

    processScene(scene, mode);
}

void Model::draw(Shader shader){
    for(Mesh mesh : meshes){
        mesh.draw(shader);
    }
}

void Model::processScene(const aiScene *scene, int mode){
    aiNode *rootNode = scene->mRootNode;
    vector<aiNode*> stack;
    aiNode *node;
    stack.push_back(rootNode);
    while(!stack.empty()){
        node = stack.back();
        stack.pop_back();
        for(int i=0; i<node->mNumMeshes; i++){
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            switch(mode){
                case  0: meshes.push_back(processSimpleMesh(mesh, scene));   break;
                case  1: meshes.push_back(processEdgeMesh(mesh, scene));     break;
            } 
        }
        for(int i=0; i<node->mNumChildren; i++){
            stack.push_back(node->mChildren[i]);
        }
    }
}

Mesh Model::processSimpleMesh(aiMesh *mesh, const aiScene *scene){
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    for(int i=0; i<mesh->mNumVertices; i++){
        Vertex vertex;
        glm::vec3 vector = to_vec3(mesh, mesh->mVertices[i]);
        vertex.position = vector;
        vector = to_vec3(mesh, mesh->mNormals[i]);
        vertex.normal = vector;
        vertices.push_back(vertex);
    }

    for(int i=0; i<mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(int j=0; j<face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    return SimpleMesh(vertices, indices);
}

Mesh Model::processEdgeMesh(aiMesh *mesh, const aiScene *scene){
    vector<EdgeVertex> edgeVertices;
    unordered_map<tuple<glm::vec3, glm::vec3>, glm::vec3, edge_hash> edgeMap;   // to store face normal
    unordered_map<glm::vec3, glm::vec3> vertexMap;                              // to store vertex normal

    for(int i=0; i<mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        glm::vec3 faceNormal = to_vec3(mesh, mesh->mNormals[i]);

        //calculating face normal
        glm::vec3 c1 = to_vec3(mesh, mesh->mVertices[face.mIndices[0]]);
        glm::vec3 c2 = to_vec3(mesh, mesh->mVertices[face.mIndices[1]]);
        glm::vec3 c3 = to_vec3(mesh, mesh->mVertices[face.mIndices[2]]);
        glm::vec3 myNormal = glm::cross(c1-c2, c1-c3);

        for(int j=0; j<face.mNumIndices; j++){
            // assign both vertices of edge
            glm::vec3 v1 = to_vec3(mesh, mesh->mVertices[face.mIndices[j]]);
            glm::vec3 v2 = to_vec3(mesh, mesh->mVertices[face.mIndices[(j+1)%face.mNumIndices]]);
            
            // add face normal to vertex normal
            if(vertexMap.find(v1) != vertexMap.end())
                vertexMap[v1] += myNormal;//faceNormal;
            else
                vertexMap[v1] = myNormal;//faceNormal;
            
            // lexically order edges
            if(v1.x > v2.x || (v1.x == v2.x && (v1.y > v2.y || (v1.y == v2.y && v1.z > v2.z)))){
                glm::vec3 temp = v1;
                v1 = v2;
                v2 = temp;
            }

            // include face normal or add edge vertices to the list
            tuple<glm::vec3, glm::vec3> edge = make_tuple(v1, v2);
            if(edgeMap.find(edge) != edgeMap.end()){
                EdgeVertex edgev;
                edgev.nA   = myNormal;//faceNormal;
                edgev.nB   = edgeMap[edge];
                for(int kind=0; kind<3; kind++){
                    edgev.kind = kind;
                    edgev.v    = v1;
                    edgev.v2   = v2;
                    edgeVertices.push_back(edgev);
                    edgev.v    = v2;
                    edgev.v2   = v1;
                    edgeVertices.push_back(edgev);
                }
                edgeMap.erase(edge);
            }
            else
                edgeMap[edge] = faceNormal;
        }
        
    }

    EdgeVertex prev;
    for(EdgeVertex& edgev : edgeVertices)
        edgev.nv = glm::normalize(vertexMap[edgev.v]);

    cout << "total vertices: " << mesh->mNumVertices << "\nunique vertices: " << vertexMap.size() << "\nedge vertices: " << edgeVertices.size() << "\nunprocessed edges: " << edgeMap.size() << "\n";
    return EdgeMesh(edgeVertices);
}
