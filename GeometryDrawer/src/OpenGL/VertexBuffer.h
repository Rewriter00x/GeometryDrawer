#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data = nullptr, unsigned int size = 0);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void Refill(const void* data, unsigned int size) const;

private:
	unsigned int m_RendererId = 0;

};

