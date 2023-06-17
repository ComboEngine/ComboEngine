#include "pch.h"
#include "TextureSerializer.h"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION 
#include <stb_image.h>
#include <nlohmann.h>

void TextureSerializer::Save(Scope<Texture> texture, std::string path)
{
	std::ofstream file;
	file.open(path, std::ios::binary | std::ios::out);

	uint32_t size = texture.Get()->Width * texture.Get()->Height * 4;
	file.write((const char*)&size, sizeof(uint32_t));

	nlohmann::json j;
	j["Width"] = texture.Get()->Width;
	j["Height"] = texture.Get()->Height;

	std::string json = j.dump();
	uint32_t jsonSize = json.size();
	file.write((const char*)&jsonSize, sizeof(uint32_t));

	file.write(json.data(), jsonSize);
	file.write((const char*)texture.Get()->MipData, size);

	file.close();
}

void TextureSerializer::Read(Scope<Texture>& texture, std::string path)
{
	std::ifstream infile;
	infile.open(path, std::ios::binary);

	infile.seekg(0);

	uint32_t size;
	infile.read((char*)&size, sizeof(uint32_t));

	uint32_t jsonSize;
	infile.read((char*)&jsonSize, sizeof(uint32_t));

	std::string json;
	json.resize(jsonSize);

	infile.read(json.data(), jsonSize);

	nlohmann::json j = nlohmann::json::parse(json);

	void* mipData = malloc(size);
	infile.read((char*)mipData, size);

	Texture::Create(texture, mipData, j["Width"], j["Height"]);
}

void TextureSerializer::Import(Scope<Texture>& texture, std::string path)
{
	int ImageWidth;
	int ImageHeight;
	int ImageChannels;

	unsigned char* ImageData = stbi_load("test.png",
		&ImageWidth,
		&ImageHeight,
		&ImageChannels, 4);

	Texture::Create(texture, ImageData, ImageWidth, ImageHeight);
}
