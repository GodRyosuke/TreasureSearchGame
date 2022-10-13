#pragma once

#include <string>
#include <vector>
#include "glm.hpp"
#include <map>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include "Component.hpp"
#include <unordered_map>

#define MAX_NUM_BONES_PER_VERTEX 4
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals |  aiProcess_JoinIdenticalVertices )
#define INVALID_MATERIAL 0xFFFFFFFF

class Mesh{
public:
    Mesh();
    ~Mesh();
    bool Load(std::string fileName, std::string ext = "fbx");
    // Texture, Diffuse, Ambient, SpecularÇ»Ç«ÇÃê›íË
    void SetTexture(std::string filePath) { mTexturePath = filePath; }
    void SetMaterialUniform(class Shader* shader);
    void BindVertexArray();
    void UnBindVertexArray();


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
    virtual void BindTexture(int materialIdx);


    const aiScene* m_pScene;
    Assimp::Importer m_Importer;    // Importerï€éùÇπÇÒÇ©Ç¡ÇΩÇÁÅASceneÇ‡ï€éùÇ≈Ç´Ç»Ç¢!!

    //Shader* mShader;

    unsigned int mNumVertices;
    unsigned int mNumIndices;
    std::vector<BasicMeshEntry> m_Meshes;


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
    enum BUFFER_TYPE {
        INDEX_BUFFER = 0,
        POS_VB = 1,
        TEXCOORD_VB = 2,
        NORMAL_VB = 3,
        NUM_BUFFERS = 4,  // required only for instancing
    };
    unsigned int mVertexArray;
    unsigned int m_Buffers[NUM_BUFFERS];




    std::vector<Material> m_Materials;
    std::vector<glm::vec3> m_Positions;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::vec2> m_TexCoords;
    std::vector<unsigned int> m_Indices;
    std::unordered_map<std::string, class Texture*> mTextures;
    std::string mTexturePath;
};
