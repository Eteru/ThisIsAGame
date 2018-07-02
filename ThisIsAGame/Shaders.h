#pragma once

#include <gl/glew.h>
#include <string>
#include <vector>
#include <map>

class Shaders 
{
public:
	static constexpr int MAX_LIGHTS = 20;

	GLint positionAttribute;
	GLint colorAttribute;
	GLint normalAttribute;
	GLint uvAttribute;
	GLint uvBlendAttribute;
	GLint texHeightsUniform;
	GLint matrixUniform;
	GLint mmatrixUniform;
	GLint nmmatrixUniform;
	GLint vmmatrixUniform;
	GLint colorUniform;
	// texture blending
	GLint texUniform[4];
	// skybox texture
	GLint cubeTexUniform;
	// fire
	GLint timeUniform;
	GLint displacementMaxUniform;
	//fog
	GLint fogAlphaUniform;
	GLint fogColorUniform;
	// light
	GLint lightCountUniform;
	GLint lightAmbientUniform;
	GLint lightAmbientRatioUniform;

	GLint lightTypeUniform[MAX_LIGHTS];
	GLint lightPositionUniform[MAX_LIGHTS];
	GLint lightAttenuationUniform[MAX_LIGHTS];
	GLint lightDirectionUniform[MAX_LIGHTS];
	GLint lightSpecularUniform[MAX_LIGHTS];
	GLint lightSpecularCoefUniform[MAX_LIGHTS];
	GLint lightSpecularShininessUniform[MAX_LIGHTS];
	GLint lightDiffuseUniform[MAX_LIGHTS];
	GLint lightDiffuseCoefUniform[MAX_LIGHTS];
	GLint lightSpotAngleUniform[MAX_LIGHTS];

	int Init(const std::string & fileVS, const std::string & fileFS);

	inline const std::map<std::string, GLint> & GetShaderMap() const
	{
		return m_map;
	}

	std::string ArrayStructUniform(std::string str_name, std::string member_name, size_t index) const;

	~Shaders();

private:
	GLuint m_program, m_vs, m_fs;

	std::map<std::string, GLint> m_map;

	int LoadShaderFile(const std::string & filepath, GLenum shader_type);
	int LoadProgram(const std::vector<GLuint> & shaders);

};