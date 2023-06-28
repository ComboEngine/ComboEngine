#include "pch.h"
#include "TextureAsset.h"
#include <Core/Material.h>
#include <Core/GlobalShaders.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void TextureAsset::ImportFromOriginal(std::string BinaryPath)
{
	int ImageWidth;
	int ImageHeight;
	int ImageChannels;

	unsigned char* ImageData = stbi_load(BinaryPath.c_str(),
		&ImageWidth,
		&ImageHeight,
		&ImageChannels, 4);

	std::ofstream file;
	file.open(OSPath.c_str(), std::ios::binary | std::ios::out);

	std::string uuid = this->UUID;
	uint32_t uuidSize = uuid.size();
	file.write((const char*)&uuidSize, sizeof(uint32_t));
	file.write(uuid.data(), uuidSize);

	uint32_t size = ImageWidth * ImageHeight * 4;
	file.write((const char*)&size, sizeof(uint32_t));

	nlohmann::json j;
	j["Width"] = ImageWidth;
	j["Height"] = ImageHeight;

	std::string json = j.dump();
	uint32_t jsonSize = json.size();
	file.write((const char*)&jsonSize, sizeof(uint32_t));

	file.write(json.data(), jsonSize);
	file.write((const char*)ImageData, size);

	file.close();

	Texture::Create(&Handle, ImageData, ImageWidth, ImageHeight);
}

void TextureAsset::CreateEmpty()
{

}

void TextureAsset::ImportFromEngineType()
{
	std::ifstream infile;
	infile.open(OSPath.c_str(), std::ios::binary);

	infile.seekg(0);

	uint32_t uuidSize;
	infile.read((char*)&uuidSize, sizeof(uint32_t));
	std::string uuid;
	uuid.resize(uuidSize);
	infile.read(uuid.data(), uuidSize);


	this->UUID = uuid;
	this->Name = std::filesystem::u8path(OSPath).filename().string().substr(0, std::filesystem::u8path(OSPath).filename().string().find_last_of("."));

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

	Texture::Create(&Handle, mipData, j["Width"], j["Height"]);
}

std::string TextureAsset::GetType()
{
	return "Texture";
}

void TextureAsset::WriteEngineFormatToStream(std::ofstream stream, std::string BinaryPath)
{
}

std::any TextureAsset::GetHandle()
{
	return this->Handle;
}

void TextureAsset::Save()
{
}
