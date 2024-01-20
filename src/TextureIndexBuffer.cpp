#include "TextureIndexBuffer.h"

TextureIndexBuffer::TextureIndexBuffer() {

    glGenBuffers(1, &gl_id);
    glBindBuffer(GL_ARRAY_BUFFER, gl_id);

    int m_instanceattribute = 7;
    glEnableVertexAttribArray(m_instanceattribute);

    glVertexAttribPointer(m_instanceattribute, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glVertexAttribDivisor(m_instanceattribute, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

TextureIndexBuffer::~TextureIndexBuffer() {
    glDeleteBuffers(1, &this->gl_id);
}

void TextureIndexBuffer::AddIndex(float index) {
    indeces->push_back(index);
}


void TextureIndexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int) * indeces->size(), &indeces->data()[0], GL_DYNAMIC_DRAW);
}

void TextureIndexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TextureIndexBuffer::SetIndecesBuffer(std::vector<float>* indeces)
{
    this->indeces = indeces;
}

int TextureIndexBuffer::NumOfIndeces()
{
    return this->indeces->size();
}
