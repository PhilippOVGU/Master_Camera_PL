#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#pragma comment(lib,"assimp.lib")


struct Position {
	float x, y, z;

};

std::vector<Position>;
std::vector<uint32_t> indices;

void processMesh(aiMesh* mesh, const aiScene* scene) {
	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Position vertex;
		vertex.x = mesh->mVertices[i].x;
		vertex.y = mesh->mVertices[i].y;
		vertex.z = mesh->mVertices[i].z;
		position.push_back(vertex);
	}
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
}

void processNode(aiNode* node, const aiScene* scene) {
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}


	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

int main(int argc, char** argv) {
	if (argc<=0)
	{
		return 1;
	}
	if (argc<2)
	{
		std::cout << "Usage: " << argv[0] << " <modelfilename>" << std::endl;
	}


	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(argv[argc - 1], aiProcess_Triangulate|aiProcess_OptimizeMeshes|aiProcess_JoinIdenticalVertices|aiProcess_ImproveCacheLocality);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE, !scene->mRootNode)
	{
		std::cout << "error while loading model with assimp: " << importer.GetErrorString() << std::endl;
		return 1;
	}

	processNode(scene->mRootNode, scene);

}