#pragma once

#include <map>
#include <string>
#include <vector>

#include "Structs.h"

class Shader
{
public:
	Shader();
	Shader(ShaderResource *sr);
	~Shader();

	void Load();

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLint GetProgramID() const
	{
		return m_program;
	}

	GLint Attrib(const GLchar* attribName) const;
	GLint Uniform(const GLchar* uniformName) const;

	bool SendAttribute(const std::string & name, uint32_t size, uint32_t stride, uint32_t offset);

	bool SendUniform(const std::string & name, int n);
	bool SendUniform(const std::string & name, float f);
	bool SendUniform(const std::string & name, glm::vec2 & vec);
	bool SendUniform(const std::string & name, glm::vec3 & vec);
	bool SendUniform(const std::string & name, glm::mat4 & mat);

	std::string CreateStructArrayName(const std::string & str_name, const std::string & member_name, size_t index) const;

private:
	bool m_loaded;
	GLuint m_program, m_vs, m_fs;
	
	std::string m_id;
	ShaderResource *m_sr;
	std::map<std::string, GLint> m_map;

	int Init(const std::string & fileVS, const std::string & fileFS);
	int LoadShaderFile(const std::string & filepath, GLenum shader_type);
	int LoadProgram(const std::vector<GLuint> & shaders);
};