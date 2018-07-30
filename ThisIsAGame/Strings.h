#pragma once
#include <string>

const std::string GAME_NAME = "This is a game";

namespace ErrorsStrings
{
	const std::string GLFW_INIT_FAILED = "GLFW instantiation failed.";
	const std::string GLFW_WINDOW_FAILED = "GLFW window creation failed.";
}

namespace ShaderStrings
{
	// Attributes
	const std::string POSITION_ATTRIBUTE = "in_posL";
	const std::string NORMAL_ATTRIBUTE = "in_normal";
	const std::string BINORM_ATTRIBUTE = "in_binorm";
	const std::string TANGENT_ATTRIBUTE = "in_tangent";
	const std::string UV_ATTRIBUTE = "in_uv";

	const std::string COLOR_ATTRIBUTE = "in_color";
	const std::string UV_BLEND_ATTRIBUTE = "in_uvBlend";
	const std::string INDICATORS_ATTRIBUTE = "in_indicators";

	// Uniforms
	const std::string MODEL_UNIFORM = "u_m";
	const std::string VIEW_MODEL_UNIFORM = "u_vm";
	const std::string NORMAL_MODEL_UNIFORM = "u_nm";
	const std::string MVP_UNIFORM = "u_matrix";
	const std::string LIGHT_SPACE_UNIFORM = "u_light_space_matrix";

	const std::string CAMERA_POSITION_UNIFORM = "u_camera_pos";

	const std::string COLOR_UNIFORM = "u_color";
	const std::string TEXTURE_0_UNIFORM = "u_texture_0";
	const std::string TEXTURE_1_UNIFORM = "u_texture_1";
	const std::string TEXTURE_2_UNIFORM = "u_texture_2";
	const std::string TEXTURE_3_UNIFORM = "u_texture_3";
	const std::string TEXTURE_CUBE_UNIFORM_0 = "u_cube_texture_0";
	const std::string TEXTURE_CUBE_UNIFORM_1 = "u_cube_texture_1";
	const std::string TEXTURE_SHADOW_MAP_UNIFORM = "u_texture_shadow_map";

	const std::string TEXTURE_UNIFORMS[] = { TEXTURE_0_UNIFORM, TEXTURE_1_UNIFORM, TEXTURE_2_UNIFORM, TEXTURE_3_UNIFORM };

	const std::string HEIGHT_MAP_UNIFORM = "u_heights";
	const std::string DISPLACEMENT_MAP_UNIFORM = "u_disp_max";
	
	// FragmentOffset
	const std::string FRAGMENT_OFFSET_X_UNIFORM = "u_fragment_offset.x";
	const std::string FRAGMENT_OFFSET_Y_UNIFORM = "u_fragment_offset.y";

	// Fog
	const std::string FOG_ALPHA_UNIFORM = "u_fog.alpha";
	const std::string FOG_COLOR_UNIFORM = "u_fog.color";

	// Lights
	const std::string LIGHT_COUNT_UNIFORM = "u_lights_count";

	const std::string LIGHT_AMBIENTAL_COLOR_UNIFORM = "u_ambiental.color";
	const std::string LIGHT_AMBIENTAL_RATIO_UNIFORM = "u_ambiental.ratio";

	const std::string LIGHT_STRUCT_NAME_UNIFORM = "u_lights";
	const std::string LIGHT_STRUCT_TYPE_UNIFORM = "type";
	const std::string LIGHT_STRUCT_POSITION_UNIFORM = "pos";
	const std::string LIGHT_STRUCT_ATTENUATION_UNIFORM = "attenuation";
	const std::string LIGHT_STRUCT_DIRECTION_UNIFORM = "dir";
	const std::string LIGHT_STRUCT_DIFFUSE_UNIFORM = "diffusal";
	const std::string LIGHT_STRUCT_DIFFUSE_RATIO_UNIFORM = "diffusal_ratio";
	const std::string LIGHT_STRUCT_SPECULAR_UNIFORM = "specular";
	const std::string LIGHT_STRUCT_SPECULAR_RATIO_UNIFORM = "specular_ratio";
	const std::string LIGHT_STRUCT_SHININESS_UNIFORM = "shininess";
	const std::string LIGHT_STRUCT_CONE_ANGLE_UNIFORM = "cone_angle";

	// Others
	const std::string TIME_UNIFORM = "u_time";
}