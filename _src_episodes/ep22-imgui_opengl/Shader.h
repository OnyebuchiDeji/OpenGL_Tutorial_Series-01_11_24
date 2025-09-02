#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//	Set Uniform
	/**
	*	If it was a more complicated shader system, like in an Engine, an implementatin using
	*	Templates would have been utilized.
	*	
	*	The SetUniform function would be templated.
	*	Or once could parse the shader source code to figure out what type a Uniform was to validate
	*	these things. So all that would be needed is `SetValue()`that just sets the name and the value of the Uniform.
	*/
	//	But this code is simpler
	void SetUniform1i(const std::string& name, int v);
	void SetUniform1f(const std::string& name, float v);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);



private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	//	Used to receive the OpenGL locations.
	int GetUniformLocation(const std::string& name);


};