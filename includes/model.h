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
        int mode;
        vector<Mesh> meshes;
        string dir;
        void processScene(const aiScene *scene, int mode);
        void processSimpleMesh(aiMesh *mesh, const aiScene *scene);
        void processEdgeMesh(aiMesh *mesh, const aiScene *scene);
        void processTextureMesh(aiMesh *mesh, const aiScene *scene);
};

#endif
