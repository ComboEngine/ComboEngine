#pragma once
#include "pch.h"
#include "Texture.h"
#include "Framebuffer.h"

class GBuffer {
public:
	Framebuffer* Position;
	Framebuffer* Diffuse;
	Framebuffer* Normal;

	Texture* PositionTexture;
	Texture* DiffuseTexture;
	Texture* NormalTexture;

	static void Create(GBuffer** Gbuffer);

	std::vector<Texture*> GetTextureArray() {
		return {
			PositionTexture,DiffuseTexture,NormalTexture
		};
	}

	virtual void ApiInit() {};
	virtual void Bind() = 0;
	virtual void Unbind() = 0;
};