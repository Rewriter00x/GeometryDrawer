#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
	void SetUniform4fv(const std::string& name, const glm::vec4& vec) const;
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix) const;

private:
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	ShaderProgramSource ParseShader(const std::string& filepath) const;
	unsigned int CompileShader(unsigned int type, const std::string& source) const;
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const;

	unsigned int GetUniformLocation(const std::string& name) const;

#if _DEBUG
	std::string m_Filepath;
#endif

	mutable std::unordered_map<std::string, unsigned int> m_UniformLocationCache;
	unsigned int m_RendererId = 0;

};

