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

public:
	VKVertex() = default;
	~VKVertex() = default;

	VKVertex(point<TValue, dimension> ptPosition, color cColor) :
		m_ptPosition(ptPosition),
		m_cColor(cColor)
	{
		//
	}

	VKVertex(std::initializer_list<TValue> ptValues, std::initializer_list<float> cValues) :
		m_ptPosition(ptValues),
		m_cColor(cValues)
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

	static EPArray<VkVertexInputAttributeDescription, 2> GetVKVertexAttributeDescriptions() {

		EPArray<VkVertexInputAttributeDescription, 2> vkVertexInputAttributeDescriptions = {};

		vkVertexInputAttributeDescriptions[0].binding = 0;
		vkVertexInputAttributeDescriptions[0].location = 0;
		vkVertexInputAttributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		vkVertexInputAttributeDescriptions[0].offset = offsetof(VKVertex, m_ptPosition);

		vkVertexInputAttributeDescriptions[1].binding = 0;
		vkVertexInputAttributeDescriptions[1].location = 1;
		vkVertexInputAttributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		vkVertexInputAttributeDescriptions[1].offset = offsetof(VKVertex, m_cColor);

		return vkVertexInputAttributeDescriptions;
	}

};

#endif // ! VULKAN_PIPELINE_H_