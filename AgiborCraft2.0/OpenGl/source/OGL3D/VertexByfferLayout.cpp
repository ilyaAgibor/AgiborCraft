#include "OGL3D/VertexBufferLayout.h"
#include <iostream>

int VertexBufferLayout::atribb = 0;

void VertexBufferLayout::AddComponent(Layout* layout) {
	this->components.push_back(layout);
	switch (layout->type)
	{
	case GL_FLOAT:
		this->stride += sizeof(float) * layout->count;
		break;
	case GL_INT:
		this->stride += sizeof(int) * layout->count;
		break;
	case GL_UNSIGNED_INT:
		this->stride += sizeof(unsigned int) * layout->count;
		break;
	default:
		this->stride += 0;
		std::cout << "Error in vertex buffer layout, no type " << layout->type << " is recognized!\n";
		break;
	}
}
  
void VertexBufferLayout::set(VertexBufferLayout& layout_buffer) {
	int start_of_next_component = 0;
	for (int i = 0; i < layout_buffer.components.size(); i++) {
		std::vector<Layout*>& comps = layout_buffer.components;
		glEnableVertexAttribArray(VertexBufferLayout::atribb);
		int local_stride = 0;
		switch (comps[i]->type)
		{
			case GL_FLOAT:
				local_stride += sizeof(float) * comps[i]->count;
				break;
			case GL_INT:
				local_stride += sizeof(int) * comps[i]->count;
				break;
			case GL_UNSIGNED_INT:
				local_stride += sizeof(unsigned int) * comps[i]->count;
				break;
			default:
				local_stride = 0;
				break;
		}
		glVertexAttribPointer(VertexBufferLayout::atribb, comps[i]->count, comps[i]->type, comps[i]->normal, layout_buffer.stride, (const void*)(start_of_next_component));
		start_of_next_component += local_stride;
		VertexBufferLayout::atribb += 1;
	}
}

VertexBufferLayout::VertexBufferLayout() {

}


VertexBufferLayout::~VertexBufferLayout() {

}