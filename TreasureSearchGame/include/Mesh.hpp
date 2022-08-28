#pragma once

#include <string>
#include <vector>
#include "glm.hpp"
#include <map>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags


#define MAX_NUM_BONES_PER_VERTEX 4
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals |  aiProcess_JoinIdenticalVertices )
#define INVALID_MATERIAL 0xFFFFFFFF

class Mesh {
public:
    Mesh();
    ~Mesh() {}
    bool Load(std::string FilePath, std::string ObjFileName);
    void Draw(class Shader* shader, float timeInSeconds);

    void SetPos(glm::vec3 pos) { mMeshPos = pos; }
    void SetRotate(glm::mat4 rot) { mMeshRotate = rot; }
    void SetScale(float scale) { mMeshScale = scale; }
    glm::vec3 GetPos() { return mMeshPos; }

    glm::mat4 GetWorldMat();

protected:
    struct BasicMeshEntry {
        BasicMeshEntry()
        {
            NumIndices = 0;
            BaseVertex = 0;
            BaseIndex = 0;
            MaterialIndex = INVALID_MATERIAL;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    virtual void PopulateBuffers();
    virtual void ReserveVertexSpace();
    virtual void LoadMesh(const aiMesh* pMesh, unsigned int meshIdx);
    virtual void GetGlobalInvTrans() {}
    virtual void UpdateTransform(class Shader* shader, float timeInSeconds);
    virtual void BindTexture(int materialIdx);

    const aiScene* m_pScene;
    Assimp::Importer m_Importer;    // Importer保持せんかったら、Sceneも保持できない!!

    //Shader* mShader;

    unsigned int mNumVertices;
    unsigned int mNumIndices;
    std::vector<BasicMeshEntry> m_Meshes;

    glm::vec3 mMeshPos;
    glm::mat4 mMeshRotate;
    float mMeshScale;


private:

    struct Material {
        glm::vec3 AmbientColor;
        glm::vec3 DiffuseColor;
        glm::vec3 SpecColor;
        float SpecPower;
        float Alpha;
        glm::vec3 Reflection;
        glm::vec3 Emissive;
        glm::vec3 Bump;
        glm::vec3 NormalMap;
        class Texture* DiffuseTexture;
    };



    unsigned int mVertexArray;


    std::vector<Material> m_Materials;
    std::vector<glm::vec3> m_Positions;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::vec2> m_TexCoords;
    std::vector<unsigned int> m_Indices;





    std::string ObjFileRoot;
    std::string ObjFileName;
};
