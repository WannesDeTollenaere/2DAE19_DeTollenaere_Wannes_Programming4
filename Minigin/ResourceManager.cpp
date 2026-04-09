#include <stdexcept>
#include <SDL3_ttf/SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include <fstream>
#include "Font.h"
#include "Helpers/Spritesheet.h"
#include <sstream>

namespace fs = std::filesystem;

void dae::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (!TTF_Init())
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	if(m_loadedTextures.find(filename) == m_loadedTextures.end())
		m_loadedTextures.insert(std::pair(filename,std::make_shared<Texture2D>(fullPath.string())));
	return m_loadedTextures.at(filename);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if(m_loadedFonts.find(key) == m_loadedFonts.end())
		m_loadedFonts.insert(std::pair(key,std::make_shared<Font>(fullPath.string(), size)));
	return m_loadedFonts.at(key);
}

std::shared_ptr<dae::SpriteSheet> dae::ResourceManager::LoadSpriteSheet(const std::string& file, int frameWidth, int frameHeight)
{
	std::string key = file + "_" + std::to_string(frameWidth) + "_" + std::to_string(frameHeight);

	auto it = m_SpriteSheets.find(key);
	if (it != m_SpriteSheets.end())
	{
		return it->second;
	}

	auto texture = LoadTexture(file);

	auto spriteSheet = std::make_shared<dae::SpriteSheet>(texture, frameWidth, frameHeight);
	m_SpriteSheets.emplace(key, spriteSheet);

	return spriteSheet;
}

nlohmann::json dae::ResourceManager::LoadJson(const std::string& file)
{
	const auto fullPath = m_dataPath / file;
	std::ifstream inputStream(fullPath);

	if (!inputStream.is_open()) 
	{
		throw std::runtime_error(std::string("Failed to load JSON file: ") + fullPath.string());
	}

	nlohmann::json jsonObject;
 	inputStream >> jsonObject;
	return jsonObject; 
}
std::string dae::ResourceManager::ReadTextFile(const std::string& filename) const
{
	std::ifstream file(m_dataPath.string() + filename);
	if (!file.is_open()) return "";

	std::stringstream ss;
	ss << file.rdbuf(); 
	return ss.str();
}

void dae::ResourceManager::WriteTextFile(const std::string& filename, const std::string& content) const
{
	std::ofstream file(m_dataPath.string() + filename);
	if (file.is_open())
	{
		file << content;
	}
}
void dae::ResourceManager::UnloadUnusedResources()
{
	for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();)
	{
		if (it->second.use_count() == 1) 
			it = m_loadedTextures.erase(it);
		else
			++it;
	}

	for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedFonts.erase(it);
		else
			++it;
	}
}
