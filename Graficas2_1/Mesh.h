#pragma once
#include "Graphics_Buffer.h"
#include "VertexType.h"
#include <d3d11.h>
#include <vector>
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using std::vector;

class Mesh
{
public:
	VertexBuffer<VertexType> m_vertexBuffer;
	IndexBuffer<unsigned int> m_indexBuffer;
	Texture m_texture;
	Mesh() = default;
	virtual ~Mesh() = default;

	void Render(ID3D11DeviceContext* pd3dImmediateContext)
	{
		unsigned int stride;
		unsigned int offset;
		stride = sizeof(VertexType);
		offset = 0;//sizeof(float);
		pd3dImmediateContext->IASetVertexBuffers(
			0, 1, &m_vertexBuffer.m_pBuffer , &stride, &offset);
		pd3dImmediateContext->IASetIndexBuffer(
			m_indexBuffer.m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
		pd3dImmediateContext->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pd3dImmediateContext->DrawIndexed(
			m_indexBuffer.Size(), 0, 0);
	}
};

class Model
{
protected:
	ID3D11Device* m_pd3dDevice = nullptr;
	vector<Mesh> m_meshes;

public:
	Model() = default;
	Model(ID3D11Device* pd3dDevice) : m_pd3dDevice(pd3dDevice) {}
	~Model() = default;

	void SetDevice(ID3D11Device* pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
	}

	void Render()
	{
		ID3D11DeviceContext* pImmContext;

		m_pd3dDevice->GetImmediateContext(&pImmContext);

		for (auto& mesh : m_meshes)
		{
			mesh.Render(pImmContext);
		}
	}

	void LoadFromFile(const char* filePath)
	{
		Assimp::Importer modelImport;
		const aiScene* scene = modelImport.ReadFile(filePath, aiProcess_Triangulate);
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			//throw std::exception(
			//	"Model::LoadFromFile - Failed to load Model");
			std::string errorMessage = modelImport.GetErrorString();
			std::cout << "Error loading model... Loading default Error Model" << std::endl;
			scene = modelImport.ReadFile("errorModel.stl", aiProcess_Triangulate);
			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				//throw std::exception("Model::LoadFromFile - Failed to load ErrorModel");
				std::cout << "Couldn't Load Error model..." << std::endl;
			}
			return;
		}
		ProcessNode(scene->mRootNode, scene);
	}
	void ProcessNode(aiNode* node, const aiScene* s)
	{
		ID3D11DeviceContext* pImmContext;
		m_pd3dDevice->GetImmediateContext(&pImmContext);
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* aimesh = s->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(ProcessMesh(aimesh, s));
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{

			ProcessNode(node->mChildren[i], s);
		}
	}
	Mesh ProcessMesh(aiMesh* aimesh, const aiScene* s)
	{
		Mesh m;
		for (unsigned int i = 0; i < aimesh->mNumVertices; ++i)
		{
			VertexType pvertex;

			pvertex.position.x = aimesh->mVertices[i].x;
			pvertex.position.y = aimesh->mVertices[i].y;
			pvertex.position.z = aimesh->mVertices[i].z;

			pvertex.normal.x = aimesh->mNormals[i].x;
			pvertex.normal.y = aimesh->mNormals[i].y;
			pvertex.normal.z = aimesh->mNormals[i].z;
			if (aimesh->mTextureCoords[0])
			{
				pvertex.u = aimesh->mTextureCoords[0][i].x;
				pvertex.v = aimesh->mTextureCoords[0][i].y;
			}
			else
			{
				pvertex.u = 0.0f; pvertex.v = 0.0f;
			}
			m.m_vertexBuffer.Add(pvertex);
		}
		for (unsigned int f = 0; f < aimesh->mNumFaces; ++f)
		{
			aiFace face = aimesh->mFaces[f];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				m.m_indexBuffer.Add(face.mIndices[j]);
			}
		}
		if (aimesh->mMaterialIndex >= 0)
		{
			//aiMaterial *material = s->mMaterials[aimesh->mMaterialIndex];
			//m_textures = loadMaterials(material, aiTextureType_SPECULAR, "texture_diffuse");
			//
			//aiMaterial *material = s->mMaterials[aimesh->mMaterialIndex];
			//m_textures = loadMaterials(material, aiTextureType_SPECULAR, "texture_diffuse");

		}
		m.m_vertexBuffer.CreateHardWareBuffer(m_pd3dDevice);
		m.m_indexBuffer.CreateHardWareBuffer(m_pd3dDevice);
		return m;
	}


	void CreateTriangle()
	{
		//to do: send every set to the render thing
		m_meshes.emplace_back();
		auto& mesh = m_meshes.back();

		mesh.m_vertexBuffer.Reserve(3);
		mesh.m_indexBuffer.Reserve(3);
		ID3D11DeviceContext* pImmContext;
		m_pd3dDevice->GetImmediateContext(&pImmContext);
		mesh.m_texture.LoadFromFile("sheikah.jpg", m_pd3dDevice, pImmContext);
		mesh.m_texture.CreateShaderSampler(m_pd3dDevice);
		mesh.m_texture.SetShaderSampler(m_pd3dDevice);
		VertexType pVertex;
		
		pVertex.position = { -1.0f,-1.0f,0.0f,1.0f };
		pVertex.color = { 1.0f,0.5f,0.25f,1.0f };
		
		pVertex.u = 0.0f ; pVertex.v = 1.0f;
		
		mesh.m_vertexBuffer.Add(pVertex);

		pVertex.position = { 0.0f,1.0f,0.0f,1.0f };
		pVertex.color = { 0.5f,1.0f, 0.0f,1.0f };		
		
		pVertex.u = 0.5f; pVertex.v = 0.0f;


		mesh.m_vertexBuffer.Add(pVertex);

		pVertex.position = { 1.0f,-1.0f,0.0f,1.0f };
		pVertex.color = { 0.25f, 0.5f, 1.0f,1.0f };
		pVertex.u = 1.0f; pVertex.v = 1.0f;


		mesh.m_vertexBuffer.Add(pVertex);

		//Deberia de tener un Load Resources el otra parte. Al cargar el modelo agarras
		//las texturas y todos sus componentes

		mesh.m_indexBuffer.Add(0);
		mesh.m_indexBuffer.Add(1);
		mesh.m_indexBuffer.Add(2);

		mesh.m_vertexBuffer.CreateHardWareBuffer(m_pd3dDevice);
		mesh.m_indexBuffer.CreateHardWareBuffer(m_pd3dDevice);
	}
	
};
