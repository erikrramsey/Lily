#include "Entity/Importer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Renderer/stb_image.h"

namespace Lily {

unsigned int TextureFromFile(const char* path);
std::string import_sub_mesh(aiMesh* mesh, aiMaterial* mat, fs::path& import_path);

Importer::Importer() = default;

void Importer::import_model(Lobject* obj, fs::path& path, const fs::path& projectPath) {
	Assimp::Importer import;
    auto flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    flags |= aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices;
	const aiScene* scene = import.ReadFile(path.string(), flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	auto directory = path.parent_path();
    fs::path import_directory = projectPath.string() + "\\imported\\";
    if (!fs::exists(import_directory)) fs::create_directory(import_directory);
    import_directory = import_directory.string() + path.filename().string() + '\\';
    fs::create_directory(import_directory);

    if (scene->mNumMeshes == 1) {
        auto mesh = scene->mMeshes[0];
        auto mat = scene->mMaterials[mesh->mMaterialIndex];

        fs::path import_path = import_directory.string() + path.filename().string();
        import_path.replace_extension(".assbin");
        auto m = Mesh(obj->m_entity, import_sub_mesh(mesh, mat, import_path));

        aiString mat_path;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &mat_path);
        if (mat_path.length == 0) m.material_path = "";
        else m.material_path = directory.string() + "\\" + std::string(mat_path.C_Str());
        load_imported_mesh(m);

        obj->add_component(m);
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[i];
        auto mat = scene->mMaterials[mesh->mMaterialIndex];

        auto mobj = obj->get_scene()->create_Lobject();
        obj->add_child(mobj);
        fs::path import_path = import_directory.string() + std::to_string(i) + path.filename().string();
        import_path.replace_extension(".assbin");
        auto m = Mesh(mobj->m_entity, import_sub_mesh(mesh, mat, import_path));

        aiString mat_path;
        mat->GetTexture(aiTextureType_DIFFUSE, 0, &mat_path);
        if (mat_path.length == 0) m.material_path = "";
        else m.material_path = directory.string() + "\\" + std::string(mat_path.C_Str());
        load_imported_mesh(m);

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

std::string import_sub_mesh(aiMesh* mesh, aiMaterial* mat, fs::path& import_path) {
    auto *mesh_copy = new aiMesh();

    mesh_copy->mNumVertices = mesh->mNumVertices;
    mesh_copy->mVertices = new aiVector3t<float>[mesh->mNumVertices];
    mesh_copy->mNormals = new aiVector3t<float>[mesh->mNumVertices];
    mesh_copy->mTextureCoords[0] = new aiVector3D[mesh->mNumVertices];
    mesh_copy->mNumUVComponents[0] = mesh->mNumVertices;
    for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            mesh_copy->mVertices[j] = mesh->mVertices[j];
        if (mesh->mNormals)
            mesh_copy->mNormals[j] = mesh->mNormals[j];
        if (mesh->mTextureCoords[0])
            mesh_copy->mTextureCoords[0][j] = mesh->mTextureCoords[0][j];
    }

    mesh_copy->mFaces = new aiFace[mesh->mNumFaces];
    mesh_copy->mNumFaces = mesh->mNumFaces;
    for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
        int inds = mesh->mFaces[j].mNumIndices;
        mesh_copy->mFaces[j].mNumIndices = inds;
        mesh_copy->mFaces[j].mIndices = new unsigned int[inds];
        for (int k = 0; k < inds; k++) {
           mesh_copy->mFaces[j].mIndices[k] = mesh->mFaces[j].mIndices[k];
        }
    }

    auto *mat_c = new aiMaterial();

    auto *root = new aiNode();
    root->mNumMeshes = 1;
    root->mMeshes = new unsigned int[] { 0 };

    auto *out = new aiScene();
    out->mNumMeshes = 1;
    out->mMeshes = new aiMesh * [] { mesh_copy };
    out->mNumMaterials = 1;
    out->mMaterials = new aiMaterial * [] { mat_c };
    out->mRootNode = root;
    out->mMetaData = new aiMetadata();

    Assimp::Exporter exporter;
    if (exporter.Export(out, "assbin", import_path.string()) != AI_SUCCESS) {
        std::cout << "ASSIMP ERROR EXPORTING FILE: " << import_path.string() << std::endl;
    }

    delete out;
    return import_path.string();
}

void Importer::load_imported_mesh(Mesh& lmesh) {
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
        glm::vec3 vector{
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        vertex.Position = vector;
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->HasTextureCoords(0)) {
            glm::vec2 vec{
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
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
        if (tex.id) {
            tex.type = "texture_diffuse";
            lmesh.textures.push_back(tex);
        }
    }
    lmesh.indices = indices;
    lmesh.vertices = vertices;
    lmesh.imported = true;
    lmesh.initMesh();
}
}
