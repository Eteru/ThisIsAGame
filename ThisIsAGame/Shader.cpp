
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Strings.h"


Shader::Shader() : m_sr(nullptr), m_loaded(false)
{
}

Shader::Shader(ShaderResource * sr) : m_sr(sr), m_loaded(false)
{
	m_id = std::stoi(sr->id);
}

Shader::~Shader()
{
	if (m_program != 0) glDeleteProgram(m_program);
	if (m_vs != 0) glDeleteShader(m_vs);
	if (m_fs != 0) glDeleteShader(m_fs);
}

void Shader::Load()
{
	if (true == m_loaded)
	{
		return;
	}

	if (nullptr == m_sr)
	{
		throw std::runtime_error(std::string("ShaderResource not available: ") + std::to_string(m_id));
	}

	if (Init(&m_sr->vs_path[0], &m_sr->fs_path[0]) < 0)
	{
		throw std::runtime_error(std::string("Could not load shader files: ") + std::to_string(m_id));
	}

	m_loaded = true;
}

bool Shader::SendAttribute(const std::string & name, uint32_t size, uint32_t stride, uint32_t offset)
{
	if (m_map[name] == -1)
	{
		//std::cerr << "Program attribute not found: " << name << std::endl;
		return false;
	}

	glEnableVertexAttribArray(m_map[name]);
	glVertexAttribPointer(m_map[name], size, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(offset));

	return true;
}

bool Shader::SendUniform(const std::string & name, int n)
{
	if (m_map[name] == -1)
	{
		//std::cerr << "Program uniform not found: " << name << std::endl;
		return false;
	}

	glUniform1i(m_map[name], n);

	return true;
}

bool Shader::SendUniform(const std::string & name, float f)
{
	if (m_map[name] == -1)
	{
		//std::cerr << "Program uniform not found: " << name << std::endl;
		return false;
	}

	glUniform1f(m_map[name], f);

	return true;
}

bool Shader::SendUniform(const std::string & name, glm::vec2 & vec)
{
	if (m_map[name] == -1)
	{
		//std::cerr << "Program uniform not found: " << name << std::endl;
		return false;
	}

	glUniform2fv(m_map[name], 1, glm::value_ptr(vec));

	return true;
}

bool Shader::SendUniform(const std::string & name, glm::vec3 & vec)
{
	if (m_map[name] == -1)
	{
		//std::cerr << "Program uniform not found: " << name << std::endl;
		return false;
	}

	glUniform3fv(m_map[name], 1, glm::value_ptr(vec));

	return true;
}

bool Shader::SendUniform(const std::string & name, glm::mat4 & mat)
{
	if (m_map[name] == -1)
	{
		//std::cerr << "Program uniform not found: " << name << std::endl;
		return false;
	}

	glUniformMatrix4fv(m_map[name], 1, GL_FALSE, glm::value_ptr(mat));

	return true;
}

std::string Shader::CreateStructArrayName(const std::string & str_name, const std::string & member_name, size_t index) const
{
	return str_name + "[" + std::to_string(index) + "]." + member_name;
}

int Shader::Init(const std::string & fileVS, const std::string & fileFS)
{
	try {
		m_vs = LoadShaderFile(fileVS, GL_VERTEX_SHADER);
		m_fs = LoadShaderFile(fileFS, GL_FRAGMENT_SHADER);
		m_program = LoadProgram({ m_vs, m_fs });

		//finding location of uniforms / attributes
		m_map[ShaderStrings::POSITION_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::POSITION_ATTRIBUTE.c_str());
		m_map[ShaderStrings::COLOR_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::COLOR_ATTRIBUTE.c_str());
		m_map[ShaderStrings::NORMAL_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::NORMAL_ATTRIBUTE.c_str());
		m_map[ShaderStrings::BINORM_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::BINORM_ATTRIBUTE.c_str());
		m_map[ShaderStrings::TANGENT_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::TANGENT_ATTRIBUTE.c_str());
		m_map[ShaderStrings::UV_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::UV_ATTRIBUTE.c_str());

		m_map[ShaderStrings::UV_BLEND_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::UV_BLEND_ATTRIBUTE.c_str());
		m_map[ShaderStrings::HEIGHT_MAP_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::HEIGHT_MAP_UNIFORM.c_str());
		m_map[ShaderStrings::MODEL_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::MODEL_UNIFORM.c_str());
		m_map[ShaderStrings::VIEW_MODEL_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::VIEW_MODEL_UNIFORM.c_str());
		m_map[ShaderStrings::NORMAL_MODEL_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::NORMAL_MODEL_UNIFORM.c_str());
		m_map[ShaderStrings::COLOR_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::COLOR_UNIFORM.c_str());
		m_map[ShaderStrings::MVP_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::MVP_UNIFORM.c_str());
		m_map[ShaderStrings::TEXTURE_0_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::TEXTURE_0_UNIFORM.c_str());
		m_map[ShaderStrings::TEXTURE_1_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::TEXTURE_1_UNIFORM.c_str());
		m_map[ShaderStrings::TEXTURE_2_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::TEXTURE_2_UNIFORM.c_str());
		m_map[ShaderStrings::TEXTURE_3_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::TEXTURE_3_UNIFORM.c_str());
		m_map[ShaderStrings::TEXTURE_CUBE_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::TEXTURE_CUBE_UNIFORM.c_str());
		m_map[ShaderStrings::TIME_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::TIME_UNIFORM.c_str());
		m_map[ShaderStrings::DISPLACEMENT_MAP_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::DISPLACEMENT_MAP_UNIFORM.c_str());
		m_map[ShaderStrings::FOG_ALPHA_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::FOG_ALPHA_UNIFORM.c_str());
		m_map[ShaderStrings::FOG_COLOR_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::FOG_COLOR_UNIFORM.c_str());
		m_map[ShaderStrings::LIGHT_AMBIENTAL_COLOR_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::LIGHT_AMBIENTAL_COLOR_UNIFORM.c_str());
		m_map[ShaderStrings::LIGHT_AMBIENTAL_RATIO_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::LIGHT_AMBIENTAL_RATIO_UNIFORM.c_str());
		m_map[ShaderStrings::LIGHT_COUNT_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::LIGHT_COUNT_UNIFORM.c_str());
		m_map[ShaderStrings::FRAGMENT_OFFSET_X_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::FRAGMENT_OFFSET_X_UNIFORM.c_str());
		m_map[ShaderStrings::FRAGMENT_OFFSET_Y_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::FRAGMENT_OFFSET_Y_UNIFORM.c_str());

		m_map[ShaderStrings::LIGHT_SPACE_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::LIGHT_SPACE_UNIFORM.c_str());
		m_map[ShaderStrings::CAMERA_POSITION_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::CAMERA_POSITION_UNIFORM.c_str());
		m_map[ShaderStrings::TEXTURE_SHADOW_MAP_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::TEXTURE_SHADOW_MAP_UNIFORM.c_str());


		for (int i = 0; i < 20; ++i) {
			std::string light_type = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_TYPE_UNIFORM, i);
			std::string light_pos = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_POSITION_UNIFORM, i);
			std::string light_attenuation = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_ATTENUATION_UNIFORM, i);
			std::string light_dir = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIRECTION_UNIFORM, i);
			std::string light_dif = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIFFUSE_UNIFORM, i);
			std::string light_dif_ratio = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIFFUSE_RATIO_UNIFORM, i);
			std::string light_spec = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SPECULAR_UNIFORM, i);
			std::string light_spec_ratio = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SPECULAR_RATIO_UNIFORM, i);
			std::string light_shininess = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SHININESS_UNIFORM, i);
			std::string light_spot_angle = CreateStructArrayName(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_CONE_ANGLE_UNIFORM, i);
		
			m_map[light_type] = glGetUniformLocation(m_program, light_type.c_str());
			m_map[light_pos] = glGetUniformLocation(m_program, light_pos.c_str());
			m_map[light_attenuation] = glGetUniformLocation(m_program, light_attenuation.c_str());
			m_map[light_dir] = glGetUniformLocation(m_program, light_dir.c_str());
			m_map[light_dif] = glGetUniformLocation(m_program, light_dif.c_str());
			m_map[light_dif_ratio] = glGetUniformLocation(m_program, light_dif_ratio.c_str());
			m_map[light_spec] = glGetUniformLocation(m_program, light_spec.c_str());
			m_map[light_spec_ratio] = glGetUniformLocation(m_program, light_spec_ratio.c_str());
			m_map[light_shininess] = glGetUniformLocation(m_program, light_shininess.c_str());
			m_map[light_spot_angle] = glGetUniformLocation(m_program, light_spot_angle.c_str());
		}
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 1;
}

int Shader::LoadShaderFile(const std::string & filepath, GLenum shader_type)
{
	std::ifstream f;
	f.open(filepath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		throw std::runtime_error(std::string("Failed to open file: ") + filepath);
	}

	//read whole file into stringstream buffer
	std::stringstream buffer;
	buffer << f.rdbuf();

	GLuint shaderID = glCreateShader(shader_type);
	if (0 == shaderID)
	{
		throw std::runtime_error("glCreateShader failed");
	}

	//set the source code
	std::string code = buffer.str();
	const char* code_c_str = code.c_str();
	glShaderSource(shaderID, 1, (const GLchar**)&code_c_str, NULL);

	//compile
	glCompileShader(shaderID);

	//throw exception if compile error occurred
	GLint status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(shaderID);
		shaderID = 0;

		throw std::runtime_error(msg);
	}

	return shaderID;
}

int Shader::LoadProgram(const std::vector<GLuint>& shaders)
{
	if (shaders.size() <= 0)
	{
		throw std::runtime_error("No shaders were provided to create the program");
	}

	//create the program object
	GLuint program = glCreateProgram();
	if (0 == program)
	{
		throw std::runtime_error("glCreateProgram failed");
	}

	//attach all the shaders
	for (unsigned i = 0; i < shaders.size(); ++i)
	{
		glAttachShader(program, shaders[i]);
	}

	//link the shaders together
	glLinkProgram(program);

	//detach all the shaders
	for (unsigned i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(program, shaders[i]);
	}

	//throw exception if linking failed
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string msg("Program linking failure: ");

		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteProgram(program);
		program = 0;

		throw std::runtime_error(msg);
	}

	return program;
}

GLint Shader::Attrib(const GLchar* attribName) const {
	if (!attribName)
		throw std::runtime_error("attribName was NULL");

	GLint attrib = glGetAttribLocation(m_program, attribName);
	if (attrib == -1)
		throw std::runtime_error(std::string("Program attribute not found: ") + attribName);

	return attrib;
}

GLint Shader::Uniform(const GLchar* uniformName) const {
	if (!uniformName)
		throw std::runtime_error("uniformName was NULL");

	GLint uniform = glGetUniformLocation(m_program, uniformName);
	if (uniform == -1)
		throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);

	return uniform;
}