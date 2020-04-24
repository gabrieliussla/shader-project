#ifndef MODEL
#define MODEL

#include "shader.h"
#include "mesh.h"

#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <string>

class Model{
    public:
        void draw();
    protected:
        vector<Mesh> meshes;
        string dir;
        void processScene(string path);
        virtual void processMesh(aiMesh *mesh, const aiScene *scene);
};

class SimpleModel : public Model{
    public:
        SimpleModel(string path);
    protected:
        virtual void processMesh(aiMesh *mesh, const aiScene *scene);
};

class EdgeModel : public Model{
    public:
        EdgeModel(string path);
    protected:
        virtual void processMesh(aiMesh *mesh, const aiScene *scene);

};

class TextureModel : public Model{
    public:
        float density;
        TextureModel(string path, float density);
    protected:
        virtual void processMesh(aiMesh *mesh, const aiScene *scene);

};

#endif
