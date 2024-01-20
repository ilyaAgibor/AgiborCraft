#pragma once
#include<vector>
#include <GLEW/glew.h>


typedef struct _Layout {
	int type;
	int count;
	bool normal;
} Layout;

class VertexBufferLayout {
public:
	static int atribb;
	VertexBufferLayout();
	~VertexBufferLayout();
	void AddComponent(Layout* layout_);
	static void set(VertexBufferLayout& layout_buffer);
private:
	int stride = 0;
	std::vector<Layout*> components;

};
