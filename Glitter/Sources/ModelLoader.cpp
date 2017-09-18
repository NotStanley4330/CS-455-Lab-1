#include "ModelLoader.h"

#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem;

glm::vec3 AiVec3ToGlm3(aiVector3D v) {
	glm::vec3 res;
	res.x = v.x;
	res.y = v.y;
	res.z = v.z;
	return res;
}

glm::vec2 AiVec3ToGlm2(aiVector3D v) {
	glm::vec2 res;
	res.x = v.x;
	res.y = v.y;
	return res;
}

static void ProcessNode(aiNode* node, const aiScene* scene, const fs::path& directory, mlModel& modelOut) {
	for (int meshIdx = 0; meshIdx < node->mNumMeshes; meshIdx++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[meshIdx]];

		mlMesh mlMesh;
	
		mlMesh.name = std::string(mesh->mName.C_Str());
		for (int vIdx = 0; vIdx < mesh->mNumVertices; vIdx++) {
			mlVertex vertex;
			vertex.position = AiVec3ToGlm3(mesh->mVertices[vIdx]);
			vertex.normal = AiVec3ToGlm3(mesh->mNormals[vIdx]);

			if (mesh->mTextureCoords[0]) {
				vertex.UV = AiVec3ToGlm2(mesh->mTextureCoords[0][vIdx]);
			}

			mlMesh.vertices.push_back(vertex);
		}

		for (int fIdx = 0; fIdx < mesh->mNumFaces; fIdx++) {
			aiFace face = mesh->mFaces[fIdx];
			for (int iIdx = 0; iIdx < face.mNumIndices; iIdx++) {
				mlMesh.indices.push_back(face.mIndices[iIdx]);
			}
		}

		// Right now this just supports a single diffuse texture per mesh.
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
			aiString str;
			mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
			fs::path tex(str.C_Str());
			mlMesh.textureFile = (directory / tex).string();
		}

		modelOut.meshes.push_back(mlMesh);
	}

	for (int cIdx = 0; cIdx < node->mNumChildren; cIdx++) {
		ProcessNode(node->mChildren[cIdx], scene, directory, modelOut);
	}
}

bool LoadModel(const std::string& directory, const std::string& modelFile, mlModel& modelOut) {
	fs::path dir(directory);
	fs::path model(modelFile);
	fs::path full = dir / model;
	Assimp::Importer imp;
	const aiScene* scene = imp.ReadFile(full.string(),
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_FlipUVs);
	
	if (!scene || !scene->mRootNode) {
		std::cout << imp.GetErrorString() << std::endl;
		return false;
	}

	ProcessNode(scene->mRootNode, scene, dir, modelOut);

	return true;
}
