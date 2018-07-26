
#include "Texture.h"
#include "TGA.h"

#include <vector>

Texture::Texture() : m_tr(nullptr), m_loaded(false)
{
}

Texture::Texture(TextureResource * tr) : m_tr(tr), m_loaded(false)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}
/// TODO: skybox & fire. trebuie inversate
void Texture::Load()
{
	if (true == m_loaded)
	{
		return;
	}

	if (nullptr == m_tr)
	{
		throw std::runtime_error(std::string("TextureResource not available: ") + std::to_string(m_id));
	}

	int err = glGetError();

	int w, h, bpp;
	char *textureBuffer = LoadTGA(m_tr->texture_path.c_str(), &w, &h, &bpp);

	glGenTextures(1, &m_id);
	glBindTexture(m_tr->type, m_id);

	if (GL_LINEAR_MIPMAP_LINEAR == m_tr->min_filter)
	{
		glGenerateMipmap(m_tr->type);
		glTexParameterf(m_tr->type, GL_TEXTURE_LOD_BIAS, -0.4f);
	}

	glTexParameteri(m_tr->type, GL_TEXTURE_WRAP_S, m_tr->wrap_s);
	glTexParameteri(m_tr->type, GL_TEXTURE_WRAP_T, m_tr->wrap_t);
	glTexParameterf(m_tr->type, GL_TEXTURE_MIN_FILTER, m_tr->min_filter);
	glTexParameterf(m_tr->type, GL_TEXTURE_MAG_FILTER, m_tr->mag_filter);	

	GLenum format = bpp == 32 ? GL_RGBA : GL_RGB;

	if (m_tr->type == GL_TEXTURE_2D) {
		glTexImage2D(m_tr->type, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, textureBuffer);
	}
	else if (m_tr->type == GL_TEXTURE_CUBE_MAP) {
		uint32_t texel_size = bpp / 8;

		std::vector<std::string> faces(6);
		
		ExtractFace(w / 2, h / 3, w / 4, h / 3, w, h, texel_size, textureBuffer, faces[0]);
		ExtractFace(0, h / 3, w / 4, h / 3, w, h, texel_size, textureBuffer, faces[1]);
		ExtractFace(w / 4, 0, w / 4, h / 3, w, h, texel_size, textureBuffer, faces[2]);
		ExtractFace(w / 4, 2 * h / 3, w / 4, h / 3, w, h, texel_size, textureBuffer, faces[3]);
		ExtractFace(w / 4, h / 3, w / 4, h / 3, w, h, texel_size, textureBuffer, faces[4]);
		ExtractFace(3 * w / 4, h / 3, w / 4, h / 3, w, h, texel_size, textureBuffer, faces[5]);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, w / 4, h / 3, 0, format, GL_UNSIGNED_BYTE, faces[0].c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, w / 4, h / 3, 0, format, GL_UNSIGNED_BYTE, faces[1].c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, w / 4, h / 3, 0, format, GL_UNSIGNED_BYTE, faces[2].c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, w / 4, h / 3, 0, format, GL_UNSIGNED_BYTE, faces[3].c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, w / 4, h / 3, 0, format, GL_UNSIGNED_BYTE, faces[4].c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, w / 4, h / 3, 0, format, GL_UNSIGNED_BYTE, faces[5].c_str());

	}

	delete textureBuffer;

	m_loaded = true;
}

// it allocates
void Texture::ExtractFace(uint32_t offsetX, uint32_t offsetY, uint32_t faceX, uint32_t faceY, uint32_t w, uint32_t h, uint32_t tx_size, char * source, std::string & destination)
{
	uint32_t buf_sz = tx_size * (w * h / 12) + 1;
	destination.resize(buf_sz);
	int c = 0;
	for (uint32_t j = offsetY; j < offsetY + faceY; ++j) {
		for (uint32_t i = offsetX; i < offsetX + faceX; ++i) {
			for (uint32_t t = 0; t < tx_size; ++t) {
				destination[c++] = source[(i + j * w) * tx_size + t];
			}
		}
	}
}

