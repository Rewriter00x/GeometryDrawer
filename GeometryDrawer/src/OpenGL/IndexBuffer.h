#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data = nullptr, unsigned int count = 0);
	~IndexBuffer();

	inline unsigned int GetCount() const { return m_Count; }

	void Bind() const;
	void Unbind() const;

	void Refill(const unsigned int* data, unsigned int count) const;

private:
	unsigned int m_RendererId = 0;
	unsigned int m_Count = 0;

};

