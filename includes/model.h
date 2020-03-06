#ifndef MODEL
#define MODEL

#include "shader.h"
#include "mesh.h"

#include <assimp/scene.h>
#include <string>

class Model{
    public:
        Model(string path, int mode);
        void draw(Shader shader);
    private:
        vector<Mesh> meshes;
        string dir;
        void processScene(const aiScene *scene, int mode);
        Mesh processSimpleMesh(aiMesh *mesh, const aiScene *scene);
        Mesh processEdgeMesh(aiMesh *mesh, const aiScene *scene);
        Mesh processTextureMesh(aiMesh *mesh, const aiScene *scene);
};

#endif
