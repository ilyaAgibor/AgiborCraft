#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "InstanceBuffer.h"
#include "TextureIndexBuffer.h"
#include "Shader.h"
#include "MVP.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Renderer {
public:	
	Renderer();
	~Renderer();
	void DrawSingleCube();
	void DrawPlayer(float x, float y, float z, float w);
	void DrawInstances();
	void SetInstancesVector(std::vector<glm::mat4>* ins);
	void SetIdecesVector(std::vector<float>* ins);
	void SetForCube();
	void Clear();
	void BackGround(float r, float g, float b, float a);
	void SetTextureSlot(int slot);
	void UpdateMVP(MVP& mvp);
	void SetSelectedCube(glm::vec3 pos);
	void BindBlock();
	void BindPlayer();
	void SetSingleCubeBuffer();
	void InstanceSetup();
	void Set();
private:
	Shader* p_shader_block;
	VertexBuffer* p_vbo_block;
	TextureIndexBuffer* p_tib_block;
	IndexBuffer* p_ibo_block;
	InstanceBuffer* p_instance_buffer_block;
	VertexBufferLayout buffer_layout_block;

	Shader* p_shader_player;
	VertexBuffer* p_vbo_player;
	IndexBuffer* p_ibo_player;
	bool player_renderer = false;
};