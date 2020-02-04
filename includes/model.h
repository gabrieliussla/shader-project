#ifndef MODEL
#define MODEL

#include "shader.h"
#include "mesh.h"

#include <assimp/scene.h>
#include <string>

class Model{
    public:
        Model(char *path);
        void draw(Shader shader);
    private:
        vector<Mesh> meshes;
        string dir;
        void loadModel(string path);
        void processScene(const aiScene *scene);
        Mesh processSimpleMesh(aiMesh *mesh, const aiScene *scene);
        Mesh processEdgeMesh(aiMesh *mesh, const aiScene *scene);
};

#endif
