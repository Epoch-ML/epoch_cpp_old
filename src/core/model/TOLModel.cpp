#include "TOLModel.h"

#include <vector>
#include <string.h>

#include "core/image/STBImage.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

RESULT TOLModel::Initialize() {
	RESULT r = R::OK;

	EPString<char> strModelFilePath = EPString<char>(EPOCH_MODELS_DIR) + "/" + m_strModelFilename;

	// load model 
	tinyobj::attrib_t tinyObjAttributes;

	// Being forced to use the STL :( 
	std::vector<tinyobj::shape_t> tinyObjShapes;
	std::vector<tinyobj::material_t> tinyObjMaterials;
	std::string strTinyObjWarning, strTinyObjError;
	
	CBM(tinyobj::LoadObj(
		&tinyObjAttributes,
		&tinyObjShapes,
		&tinyObjMaterials,
		&strTinyObjWarning,
		&strTinyObjError,
		strModelFilePath.c_str()),
	"Failed to load tiny obj file: %s warn:%s err:%s", 
		strModelFilePath.c_str(), strTinyObjWarning.c_str(), strTinyObjError.c_str());
	
	// Handle geometry

	// TODO: use indexing
	for (const auto& shape : tinyObjShapes) {
		for (const auto& index : shape.mesh.indices) {
			vertex<float, 4> vert = {};

			vert.m_ptPosition = {
				tinyObjAttributes.vertices[3 * index.vertex_index + 0],
				tinyObjAttributes.vertices[3 * index.vertex_index + 1],
				tinyObjAttributes.vertices[3 * index.vertex_index + 2],
				//tinyObjAttributes.vertex_weights[index.vertex_index]
				0.0f
			};

			vert.m_uvcoord = {
				tinyObjAttributes.texcoords[2 * index.texcoord_index + 0],
				tinyObjAttributes.texcoords[2 * index.texcoord_index + 1]
			};

			// TODO: Add normals and color
			//vert.m_vNormal = {
			//	tinyObjAttributes.normals[3 * index.vertex_index + 0],
			//	tinyObjAttributes.normals[3 * index.vertex_index + 1],
			//	tinyObjAttributes.normals[3 * index.vertex_index + 2],
			//	0.0f
			//}

			//vert.m_cColor = {
			//	tinyObjAttributes.colors[4 * index.vertex_index + 0],
			//	tinyObjAttributes.colors[4 * index.vertex_index + 1],
			//	tinyObjAttributes.colors[4 * index.vertex_index + 2],
			//	tinyObjAttributes.colors[4 * index.vertex_index + 3]
			//};

			// TODO: temp
			vert.m_cColor = color(1.0f, 1.0f, 1.0f, 1.0f);

			m_vertices.PushBack(vert);
			m_indices.PushBack(static_cast<uint32_t>(m_indices.size()));
		}
	}

	// TODO: Add material handling
	// Handle materials
	for (const auto& material : tinyObjMaterials) {
		// 
	}

	// load texture
	m_pImage = STBImage::make(m_strTextureFilename);
	CNM(m_pImage, "Failed to load texture image");

Error:
	return r;
}

RESULT TOLModel::Kill() {
	RESULT r = R::OK;

	m_pImage = nullptr;

	m_vertices.clear(true);
	m_indices.clear(true);

Error:
	return r;
}

EPRef<TOLModel> TOLModel::InternalMake(const EPString<char>& strModelFilename, const EPString<char>& strTextureFilename) {
	RESULT r = R::OK;
	EPRef<TOLModel> pTOLModel = nullptr;

	pTOLModel = new TOLModel(strModelFilename, strTextureFilename);
	CNM(pTOLModel, "Failed to allocate TOL model");

	CRM(pTOLModel->Initialize(), "Failed to initialize tol model");

Success:
	return pTOLModel;

Error:
	pTOLModel = nullptr;
	return nullptr;
}