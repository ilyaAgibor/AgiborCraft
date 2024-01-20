#include "OGL3D/Renderer.h"
float positions[] = {
	// front face		//light      //Texture UV
	0.0f, 0.0f, 1.0f,	0.9f,		 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		0.9f,		 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,		0.9f,		 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,		0.9f,		 0.0f, 1.0f,
	// back face
	0.0f, 0.0f, 0.0f,	0.6f,		 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,	0.6f,		 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		0.6f,		 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f,	0.6f,		 0.0f, 1.0f,
	// top face						 
	0.0f, 1.0f, 0.0f,	1.0f,		 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f,		1.0f,		 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,		1.0f,		 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,		1.0f,		 0.0f, 1.0f,
	// bottom face					 
	0.0f, 0.0f, 0.0f,	0.5f,		 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,	0.5f,		 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,		0.5f,		 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,	0.5f,		 0.0f, 1.0f,
	// right face					 
	1.0f, 0.0f, 0.0f,	0.8f,		 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,		0.8f,		 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,		0.8f,		 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,		0.8f,		 0.0f, 1.0f,
	// left face					 
	0.0f, 0.0f, 0.0f,	0.7f,		 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,	0.7f,		 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,		0.7f,		 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,	0.7f,		 0.0f, 1.0f
};



unsigned int indices[] = {
	// front face
	0, 1, 2,
	2, 3, 0,
	// back face
	4, 5, 6,
	6, 7, 4,
	// top face
	8, 9, 10,
	10, 11, 8,
	// bottom face
	12, 13, 14,
	14, 15, 12,
	// right face
	16, 17, 18,
	18, 19, 16,
	// left face
	20, 21, 22,
	22, 23, 20
};


float player_positions[] = {
	// front face
	-0.35, -0.35, 1.8,
	0.35, -0.35, 1.8,
	0.35, 0.35, 1.8,
	-0.35, 0.35, 1.8,
	// back face
	-0.35, -0.35, 0,
	-0.35, 0.35, 0,
	0.35, 0.35, 0,
	0.35, -0.35, 0,
	// top face
	-0.35, 0.35,0,
	-0.35, 0.35, 1.8,
	0.35, 0.35, 1.8,
	0.35, 0.35, 0,
	// bottom face
	-0.35, -0.35, 0,
	0.35, -0.35, 0,
	0.35, -0.35, 1.8,
	-0.35, -0.35, 1.8,
	// right face
	0.35, -0.35, 0,
	0.35, 0.35, 0,
	0.35, 0.35, 1.8,
	0.35, -0.35, 1.8,
	// left face
	-0.35, -0.35, 0,
	-0.35, -0.35, 1.8,
	-0.35, 0.35, 1.8,
	-0.35, 0.35, 0,
};



unsigned int player_indices[] = {
	// front face
	0, 1, 2,
	2, 3, 0,
	// back face
	4, 5, 6,
	6, 7, 4,
	// top face
	8, 9, 10,
	10, 11, 8,
	// bottom face
	12, 13, 14,
	14, 15, 12,
	// right face
	16, 17, 18,
	18, 19, 16,
	// left face
	20, 21, 22,
	22, 23, 20
};


Renderer::Renderer() {

	this->p_shader_block = new Shader("../res/shaders/Block.shader");
	this->p_shader_block->bind();
	this->p_instance_buffer_block = new InstanceBuffer();
	this->p_tib_block = new TextureIndexBuffer();
	this->p_vbo_block = new VertexBuffer(positions, sizeof(positions));
	this->p_vbo_block->bind();
	Layout vbl_pos{ GL_FLOAT, 3, 0 };
	Layout vbl_light{ GL_FLOAT, 1, 0 };
	Layout vbl_tex{ GL_FLOAT, 2, 0 };
	this->buffer_layout_block.AddComponent(&vbl_pos);
	this->buffer_layout_block.AddComponent(&vbl_light);
	this->buffer_layout_block.AddComponent(&vbl_tex);
	VertexBufferLayout::set(this->buffer_layout_block);
	this->p_ibo_block = new IndexBuffer(indices, 36);
	this->p_ibo_block->bind();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	this->p_vbo_player = new VertexBuffer(player_positions, sizeof(player_positions));
	this->p_vbo_player->bind();

	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	this->p_ibo_player = new IndexBuffer(player_indices, 36);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	this->p_shader_player = new Shader("../res/shaders/Player.shader");
	this->p_shader_player->SetUniformMat4f("pos", glm::translate(glm::mat4(1), glm::vec3(0, 0, 126)));

}
Renderer::~Renderer() {
	delete this->p_ibo_block;
	delete this->p_instance_buffer_block;
	delete this->p_tib_block;
	delete this->p_vbo_block;
	delete this->p_shader_block;
	delete this->p_ibo_player;
	delete this->p_shader_player;
	delete this->p_vbo_player;
}


void Renderer::DrawSingleCube() {
	glDrawElements(GL_TRIANGLES, this->p_ibo_block->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawPlayer(float x, float y, float z, float w)
{
	this->p_shader_player->SetUniformMat4f("pos", glm::rotate(glm::translate(glm::mat4(1), glm::vec3(x, y, z)), glm::radians(w), glm::vec3(0, 0, 1)));
	glDrawElements(GL_TRIANGLES, this->p_ibo_player->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawInstances()
{
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, this->p_instance_buffer_block->NumOfInstances());
}

void Renderer::SetInstancesVector(std::vector<glm::mat4>* ins)
{
	this->p_instance_buffer_block->SetInstacesBuffer(ins);
}

void Renderer::SetIdecesVector(std::vector<float>* ins)
{
	this->p_tib_block->SetIndecesBuffer(ins);
}



void Renderer::SetForCube()
{
	this->p_shader_block->SetUniform4f("u_Color", 0.2f, 0.8f, 0.3f, 1.0f);
}

void Renderer::Set() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::BackGround(float r, float g, float b, float a) {
	glClearColor(r,g,b,a);
}

void Renderer::SetTextureSlot(int slot)
{
	this->p_shader_block->SetUniformi("u_Texture", slot);
}

void Renderer::UpdateMVP(MVP& mvp)
{
	if(player_renderer)
		this->p_shader_player->SetUniformMat4f("u_MVP", mvp.GetMvp());
	else
		this->p_shader_block->SetUniformMat4f("u_MVP", mvp.GetMvp());
}

void Renderer::SetSelectedCube(glm::vec3 pos)
{
	this->p_shader_block->SetUniform3f("u_Selected", pos.x, pos.y, pos.z);
}

void Renderer::BindBlock()
{
	this->p_shader_block->bind();
	this->p_vbo_block->bind();
	this->p_ibo_block->bind();
	this->player_renderer = false;

}

void Renderer::BindPlayer()
{
	this->p_shader_player->bind();
	this->p_vbo_player->bind();
	this->p_ibo_player->bind();
	this->player_renderer = true;
}

void Renderer::SetSingleCubeBuffer()
{
	this->p_vbo_block->bind();
	this->p_ibo_block->bind();
}

void Renderer::InstanceSetup()
{
	this->SetSingleCubeBuffer();
	this->p_instance_buffer_block->bind();
	this->p_tib_block->bind();
}
