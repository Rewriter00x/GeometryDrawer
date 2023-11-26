#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	inline unsigned int GetCount() const { return m_Count; }

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_RendererId = 0;
	unsigned int m_Count = 0;

};

