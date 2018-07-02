
#include <iostream>
#include <fstream>
#include <sstream>

#include "Shaders.h"
#include "Strings.h"

int Shaders::Init(const std::string & fileVS, const std::string & fileFS)
{
	try {
		m_vs = LoadShaderFile(fileVS, GL_VERTEX_SHADER);
		m_fs = LoadShaderFile(fileFS, GL_FRAGMENT_SHADER);
		m_program = LoadProgram({ m_vs, m_fs });
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}


	//finding location of uniforms / attributes
	m_map[ShaderStrings::POSITION_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::POSITION_ATTRIBUTE.c_str());
	positionAttribute = m_map[ShaderStrings::POSITION_ATTRIBUTE];
	m_map[ShaderStrings::COLOR_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::COLOR_ATTRIBUTE.c_str());
	m_map[ShaderStrings::NORMAL_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::NORMAL_ATTRIBUTE.c_str());
	normalAttribute = m_map[ShaderStrings::NORMAL_ATTRIBUTE];
	m_map[ShaderStrings::BINORM_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::BINORM_ATTRIBUTE.c_str());
	m_map[ShaderStrings::TANGENT_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::TANGENT_ATTRIBUTE.c_str());

	m_map[ShaderStrings::UV_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::UV_ATTRIBUTE.c_str());
	uvAttribute = m_map[ShaderStrings::UV_ATTRIBUTE];
	m_map[ShaderStrings::UV_BLEND_ATTRIBUTE] = glGetAttribLocation(m_program, ShaderStrings::UV_BLEND_ATTRIBUTE.c_str());
	m_map[ShaderStrings::HEIGHT_MAP_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::HEIGHT_MAP_UNIFORM.c_str());
	m_map[ShaderStrings::MODEL_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::MODEL_UNIFORM.c_str());
	mmatrixUniform = m_map[ShaderStrings::MODEL_UNIFORM];
	m_map[ShaderStrings::VIEW_MODEL_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::VIEW_MODEL_UNIFORM.c_str());
	vmmatrixUniform = m_map[ShaderStrings::VIEW_MODEL_UNIFORM];
	m_map[ShaderStrings::NORMAL_MODEL_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::NORMAL_MODEL_UNIFORM.c_str());
	nmmatrixUniform = m_map[ShaderStrings::NORMAL_MODEL_UNIFORM];
	m_map[ShaderStrings::COLOR_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::COLOR_UNIFORM.c_str());
	m_map[ShaderStrings::MVP_UNIFORM] = glGetUniformLocation(m_program, ShaderStrings::MVP_UNIFORM.c_str());
	matrixUniform = m_map[ShaderStrings::MVP_UNIFORM];
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


	for (int i = 0; i < MAX_LIGHTS; ++i) {
		std::string light_type = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_TYPE_UNIFORM, i);
		std::string light_pos = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_POSITION_UNIFORM, i);
		std::string light_attenuation = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_ATTENUATION_UNIFORM, i);
		std::string light_dir = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIRECTION_UNIFORM, i);
		std::string light_dif = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIFFUSE_UNIFORM, i);
		std::string light_dif_ratio = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_DIFFUSE_RATIO_UNIFORM, i);
		std::string light_spec = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SPECULAR_UNIFORM, i);
		std::string light_spec_ratio = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SPECULAR_RATIO_UNIFORM, i);
		std::string light_shininess = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_SHININESS_UNIFORM, i);
		std::string light_spot_angle = ArrayStructUniform(ShaderStrings::LIGHT_STRUCT_NAME_UNIFORM, ShaderStrings::LIGHT_STRUCT_CONE_ANGLE_UNIFORM, i);

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
	
	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(m_program);
	glDeleteShader(m_vs);
	glDeleteShader(m_fs);
}

int Shaders::LoadShaderFile(const std::string & filepath, GLenum shader_type)
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
	const char* code = buffer.str().c_str();
	glShaderSource(shaderID, 1, (const GLchar**)&code, NULL);

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

int Shaders::LoadProgram(const std::vector<GLuint>& shaders)
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

std::string Shaders::ArrayStructUniform(std::string str_name, std::string member_name, size_t index) const
{
	return str_name + "[" + std::to_string(index) + "]." + member_name;
}
