#include "Entity/Importer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Renderer/stb_image.h"

namespace Lily {

unsigned int TextureFromFile(const char* path);
const std::string export_sub_mesh(aiMesh* mesh, aiMaterial* mat);
void        import_sub_mesh(Mesh& mesh);

Importer::Importer() = default;

void Importer::import_model(Lobject* obj, std::string& path) {
	Assimp::Importer import;
    auto flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    flags |= aiProcess_FixInfacingNormals | aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices;
	const aiScene* scene = import.ReadFile(path, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	auto directory = path.substr(0, path.find_last_of('\\'));
    auto node = scene->mRootNode;

    if (node->mNumChildren == 0) {
        auto mesh = scene->mMeshes[node->mMeshes[0]];
        auto mat = scene->mMaterials[mesh->mMaterialIndex];

        auto m = Mesh(obj->m_entity, export_sub_mesh(mesh, mat));

        aiString mat_path;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &mat_path);
        m.material_path = directory + "\\" + std::string(mat_path.C_Str());
        import_sub_mesh(m);

        obj->add_component(m);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        auto mesh = scene->mMeshes[node->mChildren[i]->mMeshes[0]];
        auto mat = scene->mMaterials[mesh->mMaterialIndex];

        auto mobj = obj->m_scene->create_Lobject();
        obj->add_child(mobj);
        auto m = Mesh(mobj->m_entity, export_sub_mesh(mesh, mat));

        aiString mat_path;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &mat_path);
        if (mat_path.length == 0) m.material_path = "";
        else m.material_path = directory + "\\" + std::string(mat_path.C_Str());
        import_sub_mesh(m);

        mobj->add_component(m);
    }

}

unsigned int TextureFromFile(const char* path) {
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (!data) {
		std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        return 0;
	}

    unsigned int textureID;
    glGenTextures(1, &textureID);

	GLenum format;
	switch (nrComponents) {
	case 1:
		format = GL_RED; break;
	case 2:
		format = GL_GREEN; break;
	case 3:
		format = GL_RGB; break;
	default:
	case 4:
		format = GL_RGBA; break;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	return textureID;
}

const std::string export_sub_mesh(aiMesh* mesh, aiMaterial* mat) {
    static int i = 0;
    aiMesh *mesh_copy = new aiMesh();
    
    mesh_copy->mNumVertices = mesh->mNumVertices;
    mesh_copy->mVertices = new aiVector3t<float>[mesh->mNumVertices];
    mesh_copy->mNormals = new aiVector3t<float>[mesh->mNumVertices];
    mesh_copy->mTextureCoords[0] = new aiVector3D[mesh->mNumVertices];
    mesh_copy->mNumUVComponents[0] = mesh->mNumVertices;
    for (int j = 0; j < mesh->mNumVertices; j++) {
        if (mesh->mTextureCoords[0]) {
            mesh_copy->mVertices[j] = mesh->mVertices[j];
            mesh_copy->mNormals[j] = mesh->mNormals[j];
            mesh_copy->mTextureCoords[0][j] = mesh->mTextureCoords[0][j];
        }
    }

    mesh_copy->mFaces = new aiFace[mesh->mNumFaces];
    mesh_copy->mNumFaces = mesh->mNumFaces;
    for (int j = 0; j < mesh->mNumFaces; j++) {
        int inds = mesh->mFaces[j].mNumIndices;
        mesh_copy->mFaces[j].mNumIndices = inds;
        mesh_copy->mFaces[j].mIndices = new unsigned int[inds];
        for (int k = 0; k < inds; k++) {
           mesh_copy->mFaces[j].mIndices[k] = mesh->mFaces[j].mIndices[k];
        }
    }

    aiMaterial *mat_c = new aiMaterial();


    aiNode *root = new aiNode();
    root->mNumMeshes = 1;
    root->mMeshes = new unsigned [] { 0 };

    aiScene *out = new aiScene();
    out->mNumMeshes = 1;
    out->mMeshes = new aiMesh * [] { mesh_copy };
    out->mNumMaterials = 1;
    out->mMaterials = new aiMaterial * [] { mat_c };
    out->mRootNode = root;
    out->mMetaData = new aiMetadata();

    Assimp::Exporter exporter;
    const std::string p = fs::current_path().string() + "\\ExportTest\\test" + std::to_string(i) + ".assbin";
    if (exporter.Export(out, "assbin", p) != AI_SUCCESS) {
        std::cout << "ASSIMP ERROR EXPORTING FILE: " << p << std::endl;
    }

    delete out;
    i++;
    return p;
}

void Importer::import_sub_mesh(Mesh& lmesh) {
    Assimp::Importer imp;
    auto scene = imp.ReadFile(lmesh.import_path, 0);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        std::cout << "ERROR: ASSIMP" << imp.GetErrorString() << std::endl;
        return;
    }
    auto mesh = scene->mMeshes[0];
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->HasTextureCoords(0)) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        if (mesh->HasTangentsAndBitangents()) {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Texture tex;
    if (!lmesh.material_path.empty()) {
        tex.id = TextureFromFile(lmesh.material_path.c_str());
        tex.type = "texture_diffuse";
        lmesh.textures.push_back(tex);
    }
    lmesh.indices = indices;
    lmesh.vertices = vertices;
    lmesh.imported = true;
    lmesh.initMesh();
}
}
