#ifndef VK_VERTEX_H_
#define VK_VERTEX_H_

#include "core/ehm/ehm.h"

// epoch Vulkan vertex
// epoch/src/hal/vulkan/VKVertex.h

#include "hal/vertex.h"

#include <vulkan/vulkan.h>

#include "core/types/EPArray.h"

template <typename TValue, int dimension>
class VKVertex 
	//: public vertex
{
public:
	point<TValue, dimension> m_ptPosition;
	color m_cColor;
	point<float, 2> m_uvcoord;

public:
	VKVertex() = default;
	~VKVertex() = default;

	VKVertex(point<TValue, dimension> ptPosition, color cColor, point<float, 2> uvcoord) :
		m_ptPosition(ptPosition),
		m_cColor(cColor),
		m_uvcoord(uvcoord)
	{
		//
	}

	VKVertex(const vertex<TValue, dimension>& rhs) :
		m_ptPosition(rhs.m_ptPosition),
		m_cColor(rhs.m_cColor),
		m_uvcoord(rhs.m_uvcoord)
	{
		//
	}

	VKVertex& operator=(const vertex<TValue, dimension>& rhs) {
		m_ptPosition = rhs.m_ptPosition;
		m_cColor = rhs.m_cColor;
		m_uvcoord = rhs.m_uvcoord;

		return *this;
	}

	VKVertex(vertex<TValue, dimension>& rhs) :
		m_ptPosition(rhs.m_ptPosition),
		m_cColor(rhs.m_cColor),
		m_uvcoord(rhs.m_uvcoord)
	{
		//
	}

	VKVertex& operator=(vertex<TValue, dimension>& rhs) {
		m_ptPosition = rhs.m_ptPosition;
		m_cColor = rhs.m_cColor;
		m_uvcoord = rhs.m_uvcoord;

		return *this;
	}

	VKVertex(
		std::initializer_list<TValue> ptValues, 
		std::initializer_list<float> cValues,
		std::initializer_list<float> uvValues
	) :
		m_ptPosition(ptValues),
		m_cColor(cValues),
		m_uvcoord(uvValues)
	{
		//
	}

	static VkVertexInputBindingDescription GetVKVertexBindingDescription() {
		VkVertexInputBindingDescription vkVertexInputBindingDescription = {};

		vkVertexInputBindingDescription.binding = 0;
		vkVertexInputBindingDescription.stride = sizeof(VKVertex);
		vkVertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return vkVertexInputBindingDescription;
	}

	static EPArray<VkVertexInputAttributeDescription, 3> GetVKVertexAttributeDescriptions() {

		EPArray<VkVertexInputAttributeDescription, 3> vkVertexInputAttributeDescriptions = {};

		vkVertexInputAttributeDescriptions[0].binding = 0;
		vkVertexInputAttributeDescriptions[0].location = 0;

		switch (dimension) {
			case 1: vkVertexInputAttributeDescriptions[0].format = VK_FORMAT_R32_SFLOAT; break;
			case 2: vkVertexInputAttributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT; break;
			case 3: vkVertexInputAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; break;
			case 4: vkVertexInputAttributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT; break;
		}
		
		vkVertexInputAttributeDescriptions[0].offset = offsetof(VKVertex, m_ptPosition);

		vkVertexInputAttributeDescriptions[1].binding = 0;
		vkVertexInputAttributeDescriptions[1].location = 1;
		vkVertexInputAttributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		vkVertexInputAttributeDescriptions[1].offset = offsetof(VKVertex, m_cColor);

		vkVertexInputAttributeDescriptions[2].binding = 0;
		vkVertexInputAttributeDescriptions[2].location = 2;
		vkVertexInputAttributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		vkVertexInputAttributeDescriptions[2].offset = offsetof(VKVertex, m_uvcoord);

		return vkVertexInputAttributeDescriptions;
	}

};

#endif // ! VULKAN_PIPELINE_H_