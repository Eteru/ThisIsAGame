#pragma once

#include <string>
#include "Structs.h"

class Texture
{
public:
	Texture();
	Texture(TextureResource *tr);
	~Texture();

	void Load();

	inline bool IsLoaded()
	{
		return m_loaded;
	}

	inline GLuint GetID() const
	{
		return m_id;
	}

	inline GLenum GetTextureType() const
	{
		return m_tr->type;
	}

private:
	bool m_loaded;
	GLuint m_id;

	TextureResource *m_tr;

	void ExtractFace(uint32_t offsetX, uint32_t offsetY, uint32_t faceX, uint32_t faceY, uint32_t w, uint32_t h, uint32_t tx_size, char * source, std::string & destination);
};